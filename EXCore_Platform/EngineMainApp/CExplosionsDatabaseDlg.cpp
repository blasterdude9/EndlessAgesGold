#include "cexplosionsdatabasedlg.h"

///////////////////////////////////////////////////////////////////////////////
// Class:		CExplosionsDatabaseDlg
// Function:	MACROS
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CExplosionsDatabaseDlg, CDialog)

BEGIN_MESSAGE_MAP(CExplosionsDatabaseDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_EXP_AddExplosion, OnBTNEXPAddExplosion)
	ON_BN_CLICKED(IDC_BTN_EXP_LoadExplosions, OnBTNEXPLoadExplosions)
	ON_BN_CLICKED(IDC_BTN_EXP_SaveExplosions, OnBTNEXPSaveExplosions)
	ON_BN_CLICKED(IDC_BTN_EXP_DeleteExplosion, OnBTNEXPDeleteExplosions)
	ON_BN_CLICKED(IDC_BTN_EXP_EditExplosionObj, OnBTNEXPEditExplosionObj)
	ON_BN_CLICKED(IDC_BTN_EXP_SaveUnit, OnBTNEXPSaveUnit)
	ON_BN_CLICKED(IDC_BTN_EXP_LoadUnit, OnBTNEXPLoadUnit)
	ON_BN_CLICKED(IDC_BUTTON_explosionTest, OnBTNEXPTest)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// Class:		CExplosionsDatabaseDlg
// Function:	Ctor/Dtor
///////////////////////////////////////////////////////////////////////////////

CExplosionsDatabaseDlg::CExplosionsDatabaseDlg(CWnd* pParent) 
	: CDialog(CExplosionsDatabaseDlg::IDD, pParent)
{
	
}

CExplosionsDatabaseDlg::~CExplosionsDatabaseDlg()
{
	
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CExplosionsDatabaseDlg
// Function:	OnInitDialog()
// Description:	Inherited from CDialog.
///////////////////////////////////////////////////////////////////////////////

BOOL
CExplosionsDatabaseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->InListExplosionDB(this->m_pStatus->m_explosionDBList->GetCount() - 1);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CExplosionsDatabaseDlg
// Function:	OnOK()
// Description:	Inherited from CDialog.
///////////////////////////////////////////////////////////////////////////////

void
CExplosionsDatabaseDlg::OnOK()
{
	CDialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CExplosionsDatabaseDlg
// Function:	DoDataExchange(CDataExchange*)
// Description:	Inherited from CDialog. 
//				No control data members at this time.
///////////////////////////////////////////////////////////////////////////////

void
CExplosionsDatabaseDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CExplosionsDatabaseDlg
// Function:	OnBTNEXPAddExplosion()
// Description:	
///////////////////////////////////////////////////////////////////////////////

void
CExplosionsDatabaseDlg::OnBTNEXPAddExplosion() 
{
#ifndef _ClientOutput
	CExplosionBasicDlg dlg;
	CExplosionObj *newExpObj = new CExplosionObj();

	dlg.g_pd3dDeviceRef = m_pStatus->m_dxRenderer->m_pd3dDevice;
	dlg.m_dirSndRef = m_pStatus->m_dirSnd;
	dlg.m_explosionRef = newExpObj;
	dlg.GL_textureDataBase = m_pStatus->m_textureDataBase;
	dlg.m_elementDBREF = m_pStatus->m_elementDB;
	dlg.m_skillDatabaseRef = m_pStatus->m_skillDatabase;

	if(dlg.DoModal() == IDOK)
	{
		m_pStatus->m_explosionDBList->AddTail(newExpObj);
		InListExplosionDB((m_pStatus->m_explosionDBList->GetCount() - 1));
		if(newExpObj->m_meshObject)
			m_pStatus->ReinitTextureUsage(newExpObj->m_meshObject);
	}
	else
	{
		SafeDelete(newExpObj);
		/* DEPRECATED /////////////////////////////////////////////////////////////
		newExpObj->SafeDelete();
		delete newExpObj;
		newExpObj = 0;
		/////////////////////////////////////////////////////////////////////////*/
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CExplosionsDatabaseDlg
// Function:	OnBTNEXPDeleteExplosions()
// Description:	
///////////////////////////////////////////////////////////////////////////////

void 
CExplosionsDatabaseDlg::OnBTNEXPDeleteExplosions() 
{
	int runtimeIndex = this->SendDlgItemMessage(IDC_LIST_ExplosionList,LB_GETCURSEL ,0,0);
	if(runtimeIndex == -1)
	{
		MessageBox("Must select an explosion");
		return;
	}
	POSITION posLoc = m_pStatus->m_explosionDBList->FindIndex(runtimeIndex);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CExplosionObj *expObj = (CExplosionObj*)m_pStatus->m_explosionDBList->GetAt(posLoc);
	SafeDelete(expObj);
	/* DEPRECATED /////////////////////////////////////////////////////////////
	expObj->SafeDelete();
	delete expObj;
	expObj=0;
	/////////////////////////////////////////////////////////////////////////*/
	m_pStatus->m_explosionDBList->RemoveAt(posLoc);

	InListExplosionDB(runtimeIndex-1);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CExplosionsDatabaseDlg
// Function:	OnBTNEXPEditExplosionObj()
// Description:	
///////////////////////////////////////////////////////////////////////////////

void
CExplosionsDatabaseDlg::OnBTNEXPEditExplosionObj() 
{
#ifndef _ClientOutput
	int runtimeIndex = this->SendDlgItemMessage(IDC_LIST_ExplosionList, LB_GETCURSEL, 0, 0);
	if(runtimeIndex == -1)
	{
		MessageBox("Must select an explosion");
		return;
	}
	POSITION posLoc = m_pStatus->m_explosionDBList->FindIndex(runtimeIndex);
	if(!posLoc) return;
	CExplosionObj *expObj = (CExplosionObj*)m_pStatus->m_explosionDBList->GetAt(posLoc);
	CExplosionBasicDlg dlg;
	dlg.m_dirSndRef = m_pStatus->m_dirSnd;
	dlg.g_pd3dDeviceRef = m_pStatus->m_dxRenderer->m_pd3dDevice;
	dlg.m_explosionRef = expObj;
	dlg.m_elementDBREF = m_pStatus->m_elementDB;
	dlg.GL_textureDataBase = m_pStatus->m_textureDataBase;
	dlg.m_skillDatabaseRef = m_pStatus->m_skillDatabase;
	if(dlg.DoModal()==IDOK){}
	InListExplosionDB(runtimeIndex);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CExplosionsDatabaseDlg
// Function:	OnBTNEXPLoadExplosions()
// Description:	
///////////////////////////////////////////////////////////////////////////////

void 
CExplosionsDatabaseDlg::OnBTNEXPLoadExplosions() 
{
	// Use gamefiles folder for organization
	CString CurDir;
	CString MainDir;
	char buffer[_MAX_PATH];
	_getcwd(buffer, _MAX_PATH);
	MainDir = CurDir = buffer;
	CurDir = CurDir + "\\GameFiles\\";
	SetCurrentDirectory(CurDir);

	static char BASED_CODE filter[] = "EXP Files|*.EXP|All Files (*.*)|*.*||";
	CFileDialog opendialog(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this);
	if(opendialog.DoModal() == IDOK)
	{
		CString fileName = opendialog.GetFileName();
		SetCurrentDirectory(MainDir);
		m_pStatus->LoadExplosionFile(fileName);
		InListExplosionDB(0);
	}
	SetCurrentDirectory(MainDir);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CExplosionsDatabaseDlg
// Function:	OnBTNEXPSaveExplosions()
// Description:	
///////////////////////////////////////////////////////////////////////////////

void 
CExplosionsDatabaseDlg::OnBTNEXPSaveExplosions() 
{
#ifndef _LimitedEngine
 //use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "EXP Files|*.EXP|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "EXP", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
       if(res == FALSE){
		  SetCurrentDirectory(MainDir);
		  MessageBox(fileName);
	      return;
	   }
       CArchive the_out_Archive(&fl,CArchive::store);
       the_out_Archive << m_pStatus->m_explosionDBList;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);  
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CExplosionsDatabaseDlg
// Function:	OnBTNEXPSaveUnit()
// Description:	
///////////////////////////////////////////////////////////////////////////////

void
CExplosionsDatabaseDlg::OnBTNEXPSaveUnit() 
{
#ifndef _LimitedEngine
   int runtimeIndex = this->SendDlgItemMessage(IDC_LIST_ExplosionList,LB_GETCURSEL ,0,0);
   if(runtimeIndex == -1){
      MessageBox("Must select an explosion");
	  return;
   }
   POSITION posLoc = m_pStatus->m_explosionDBList->FindIndex(runtimeIndex);
   if(!posLoc)
	   return;
       CExplosionObj * expObj = (CExplosionObj *)m_pStatus->m_explosionDBList->GetAt(posLoc);
	   
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "EXU Files|*.EXU|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "EXU", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << expObj;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir); 
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CExplosionsDatabaseDlg
// Function:	OnBTNEXPLoadUnit()
// Description:	
///////////////////////////////////////////////////////////////////////////////

void
CExplosionsDatabaseDlg::OnBTNEXPLoadUnit() 
{
	// Use gamefiles folder for organization
	CString CurDir;
	CString MainDir;
	char buffer[_MAX_PATH];
	_getcwd(buffer, _MAX_PATH);
	MainDir = CurDir = buffer;
	CurDir = CurDir + "\\GameFiles\\";
	SetCurrentDirectory(CurDir);

	static char BASED_CODE filter[] = "EXU Files|*.EXU|All Files (*.*)|*.*||";
	CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
	if ( opendialog.DoModal() == IDOK )
	{
		CString fileName = opendialog.GetFileName();
		CFileException exc;
		CFile fl;
		BOOL res = fl.Open(fileName, CFile::modeRead, &exc);
		if(res == FALSE)
		{
			SetCurrentDirectory(MainDir);
			return;
		}
		CExplosionObj *expObj = NULL;
		CArchive the_in_Archive(&fl,CArchive::load);
		the_in_Archive >> expObj;
		the_in_Archive.Close();
		fl.Close();
		m_pStatus->m_explosionDBList->AddTail(expObj);
	}
	InListExplosionDB(m_pStatus->m_explosionDBList->GetCount()-1);
	SetCurrentDirectory(MainDir);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CExplosionsDatabaseDlg
// Function:	OnBTNEXPTest()
// Description:	
///////////////////////////////////////////////////////////////////////////////

void 
CExplosionsDatabaseDlg::OnBTNEXPTest()
{
   int runtimeIndex = this->SendDlgItemMessage(IDC_LIST_ExplosionList,LB_GETCURSEL ,0,0);
   if(runtimeIndex == -1){
      MessageBox("Must select an explosion");
	  return;
   }
   
   D3DVECTOR orientationDir;
   ZeroMemory(&orientationDir,sizeof(D3DVECTOR));
   orientationDir.y = 1.0f;

   POSITION entPosition = m_pStatus->m_entObjRuntime->FindIndex(0);

   if(!entPosition)
   {
		AfxMessageBox("entity must be spawned first-uses first one in list");
		return;
   }

   CMovementObj * ptrLoc = ( CMovementObj* ) m_pStatus->m_entObjRuntime->GetAt(entPosition);
	
   D3DVECTOR intersect;
   ptrLoc->List_baseFrame->GetPosition(NULL,&intersect);

   /* DEPRECATED //////////////////////////////////////////////////////////////
   m_pStatus->m_explosionDBList->CreateExplosion(runtimeIndex,
										         intersect,
										         m_pStatus->m_explosionRuntimeList,
										         orientationDir,
												 -1,
												 m_pStatus->m_dxRenderer->m_pd3dDevice,
												 0,
											     -1
										         ); 
	/////////////////////////////////////////////////////////////////////////*/

   CExplosionObj * expObj = m_pStatus->m_explosionDBList->GetPtr(runtimeIndex);
   if(expObj)
	  m_pStatus->DoExpEffect(runtimeIndex,0,m_pStatus->m_explosionRuntimeList,expObj->m_pushPower,expObj->m_damageRadius);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CExplosionsDatabaseDlg
// Function:	InListExplosionDB(int)
// Description:	
///////////////////////////////////////////////////////////////////////////////

void
CExplosionsDatabaseDlg::InListExplosionDB(int sel)
{
	int traceIt=0;
	this->SendDlgItemMessage(IDC_LIST_ExplosionList, LB_RESETCONTENT, 0, 0);	
	for(POSITION posLoc = m_pStatus->m_explosionDBList->GetHeadPosition();posLoc != NULL;)
	{
		CExplosionObj *partPtr = (CExplosionObj*)m_pStatus->m_explosionDBList->GetNext(posLoc);

		CString build = operator+(" ", partPtr->m_explosionName);
		build = operator+(m_pStatus->m_cabLib.ConvToString(traceIt),build);
		this->SendDlgItemMessage(IDC_LIST_ExplosionList, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)build);
		traceIt++;
	}
	if(sel != -1) this->SendDlgItemMessage(IDC_LIST_ExplosionList, LB_SETCURSEL , sel, 0);
}
