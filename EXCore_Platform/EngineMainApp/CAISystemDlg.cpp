#include "CAISystemDlg.h"

///////////////////////////////////////////////////////////////////////////////
// Class:		CAISystemDlg
// Function:	MACROS
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CAISystemDlg, CDialog)

BEGIN_MESSAGE_MAP(CAISystemDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_AI_CreateNew, OnBTNAICreateNew)
	ON_BN_CLICKED(IDC_BTN_AI_Delete, OnBTNAIDelete)
	ON_BN_CLICKED(IDC_BTN_AI_AddEvent, OnBTNAIAddEvent)
	ON_BN_CLICKED(IDC_BTN_AI_AddScript, OnBTNAIAddScript)
	ON_BN_CLICKED(IDC_BTN_AI_EditScript, OnBTNAIEditScript)
	ON_BN_CLICKED(IDC_BTN_AI_DeleteScript, OnBTNAIDeleteScript)
	ON_BN_CLICKED(IDC_BTN_AI_EditEvent, OnBTNAIEditEvent)
	ON_BN_CLICKED(IDC_BTN_AI_DeleteEvent, OnBTNAIDeleteEvent)
	ON_BN_CLICKED(IDC_BTN_AI_AddTree, OnBTNAIAddTree)
	ON_BN_CLICKED(IDC_BTN_AI_DeleteTree, OnBTNAIDeleteTree)
	ON_BN_CLICKED(IDC_BTN_AI_Edit, OnBTNAIEdit)
	ON_BN_CLICKED(IDC_BTN_AI_SaveAiData, OnBTNAISaveAiData)
	ON_BN_CLICKED(IDC_BTN_AI_LoadAiData, OnBTNAILoadAiData)
	ON_BN_CLICKED(IDC_BTN_AI_EditAITree, OnBTNAIEditAITree)
	ON_BN_CLICKED(IDC_BTN_AI_LoadAiUnit, OnBTNAILoadAiUnit)
	ON_BN_CLICKED(IDC_BTN_AI_SaveAiUnit, OnBTNAISaveAiUnit)
	ON_BN_CLICKED(IDC_BTN_AI_ImportCollisionSys, OnBTNAIImportCollisionSys)
	ON_BN_CLICKED(IDC_BTN_AI_DeleteSys, OnBTNAIDeleteSys)
	ON_BN_CLICKED(IDC_BTN_AI_ExportCurrentCol, OnBTNAIExportCurrentCol)
	ON_BN_CLICKED(IDC_BTN_AI_SaveColDB, OnBTNAISaveColDB)
	ON_BN_CLICKED(IDC_BTN_AI_LoadColDB, OnBTNAILoadColDB)
	ON_BN_CLICKED(IDC_BTN_AI_IntegrityCheck, OnBTNAIIntegrityCheck)
	ON_BN_CLICKED(IDC_BTN_AI_newTree, OnBTNAInewTree)
	ON_BN_CLICKED(IDC_BTN_AI_newTree, OnBTNAInewTree)
	ON_BN_CLICKED(IDC_BTN_UI_AIRainSys, OnBTNUIAIRainSys)
	ON_BN_CLICKED(IDC_BTN_AI_Integrity_Check, OnBTNAIIntegrityCheck)
	ON_BN_CLICKED(IDC_BUTTON_AIExportToSql, OnBTNAIExportToSql)
	ON_LBN_SELCHANGE(IDC_LIST_AI_Scripts, OnSelchangeLISTAIScripts)
	ON_LBN_SELCHANGE(IDC_LIST_AI_WebTrees, OnSelchangeLISTAItree)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// Class:		CAISystemDlg
// Function:	Ctor/Dtor
///////////////////////////////////////////////////////////////////////////////

CAISystemDlg::CAISystemDlg(CWnd* pParent) 
	: CDialog(CAISystemDlg::IDD, pParent)
{
	
}

CAISystemDlg::~CAISystemDlg()
{
	
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CAISystemDlg
// Function:	OnInitDialog()
// Description:	Inherited from CDialog.
///////////////////////////////////////////////////////////////////////////////

BOOL
CAISystemDlg::OnInitDialog()
{
	this->AITreeInList(0);
	this->ScriptInList(0);
	this->AI_DBInList(0);
	CDialog::OnInitDialog();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CAISystemDlg
// Function:	OnOK()
// Description:	Inherited from CDialog.
///////////////////////////////////////////////////////////////////////////////

void
CAISystemDlg::OnOK()
{
	CDialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CAISystemDlg
// Function:	DoDataExchange(CDataExchange*)
// Description:	Inherited from CDialog. 
//				No control data members at this time.
///////////////////////////////////////////////////////////////////////////////

void
CAISystemDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CAISystemDlg::OnBTNAIAddEvent() 
{
#ifndef _ClientOutput
	int entIndex = this->SendDlgItemMessage(IDC_LIST_AI_Scripts,LB_GETCURSEL ,0,0);
	if(entIndex == -1){
	    MessageBox("Must select a script");
		return;
	}
	CScriptEventAdd dlg;
	dlg.m_finalEventInt = 0;
	if(dlg.DoModal() == IDOK){
       POSITION posIndex = m_pStatus->m_scrList->FindIndex(entIndex);
	   if(posIndex){
          CAIScriptObj * script = (CAIScriptObj *)m_pStatus->m_scrList->GetAt(posIndex);
          if(!script->m_events)
			  script->m_events = new CAIScriptEventList();
		      script->m_events->AddEvent(dlg.m_finalEventInt,dlg.m_miscInt);
			  ScriptEventsInList(script->m_events->GetCount()-1);
	   }
	}
#endif
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CAISystemDlg::OnBTNAIAddScript() 
{
#ifndef _ClientOutput
	CScriptAdd dlg;
	if(dlg.DoModal() == IDOK){
       CAIScriptObj * newScript = new CAIScriptObj();
	   newScript->scriptName        = dlg.m_scriptName;
	   newScript->scriptDescription = dlg.m_scriptDescription;

	   m_pStatus->m_scrList->AddTail(newScript);
	   ScriptInList(m_pStatus->m_scrList->GetCount()-1);
	}
#endif
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CAISystemDlg::ScriptInList(int sel)
{
	//
	this->SendDlgItemMessage(IDC_LIST_AI_Scripts,LB_RESETCONTENT,0,0);
	for(POSITION posLoc = m_pStatus->m_scrList->GetHeadPosition();posLoc != NULL;)
	{
        CAIScriptObj * newScript = (CAIScriptObj *)m_pStatus->m_scrList->GetNext(posLoc);
        this->SendDlgItemMessage(IDC_LIST_AI_Scripts,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)newScript->scriptName);
	}
	if(sel != -1)
	   this->SendDlgItemMessage(IDC_LIST_AI_Scripts,LB_SETCURSEL ,sel,0);
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CAISystemDlg::ScriptEventsInList(int sel)
{
	int entIndex = this->SendDlgItemMessage(IDC_LIST_AI_Scripts,LB_GETCURSEL ,0,0);
	if(entIndex == -1){
		return;
	}
	POSITION posIndex = m_pStatus->m_scrList->FindIndex(entIndex);
	 if(posIndex){
        CAIScriptObj * script = (CAIScriptObj *)m_pStatus->m_scrList->GetAt(posIndex);
	    this->SendDlgItemMessage(IDC_LIST_AI_Events,LB_RESETCONTENT,0,0);
		if(script->m_events)
	    for(POSITION posLoc = script->m_events->GetHeadPosition();posLoc != NULL;)
		{
            CAIScriptEvent * scEvent = (CAIScriptEvent *)script->m_events->GetNext(posLoc);
			if(scEvent->m_event == 0)
               this->SendDlgItemMessage(IDC_LIST_AI_Events,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)operator+("GoTo :",m_pStatus->m_cabLib.ConvToString(scEvent->m_miscInt)));
		    if(scEvent->m_event == 1)
               this->SendDlgItemMessage(IDC_LIST_AI_Events,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)operator+("Delay :",m_pStatus->m_cabLib.ConvToString(scEvent->m_miscInt)));
		    if(scEvent->m_event == 2)
               this->SendDlgItemMessage(IDC_LIST_AI_Events,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)operator+("Play Animation :",m_pStatus->m_cabLib.ConvToString(scEvent->m_miscInt)));
		    if(scEvent->m_event == 3)
               this->SendDlgItemMessage(IDC_LIST_AI_Events,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"GoTo Random :");
		    if(scEvent->m_event == 4)
               this->SendDlgItemMessage(IDC_LIST_AI_Events,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Reset Script :");
		}
	    if(sel != -1)
	        this->SendDlgItemMessage(IDC_LIST_AI_Events,LB_SETCURSEL ,sel,0);
	 }
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CAISystemDlg::OnBTNAIEditScript() 
{
#ifndef _ClientOutput
    CScriptAdd dlg;
	int scrIndex = this->SendDlgItemMessage(IDC_LIST_AI_Scripts,LB_GETCURSEL ,0,0);
	if(scrIndex == -1){
		MessageBox("Must select a script");
		return;
	}
	POSITION posIndex = m_pStatus->m_scrList->FindIndex(scrIndex);
	 if(posIndex){
        CAIScriptObj * script = (CAIScriptObj *)m_pStatus->m_scrList->GetAt(posIndex);
		   dlg.m_scriptName = script->scriptName;
	       dlg.m_scriptDescription = script->scriptDescription;
	    if(dlg.DoModal() == IDOK){
	       script->scriptName        = dlg.m_scriptName;
	       script->scriptDescription = dlg.m_scriptDescription;
	       ScriptInList(scrIndex);
		}	
	 }
#endif
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CAISystemDlg::OnBTNAIDeleteScript() 
{
	int scrIndex = this->SendDlgItemMessage(IDC_LIST_AI_Scripts,LB_GETCURSEL ,0,0);
	if(scrIndex == -1)
	{
		MessageBox("Must select a script");
		return;
	}
	
	POSITION posIndex = m_pStatus->m_scrList->FindIndex(scrIndex);
	if(posIndex)
	{
		if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
			return;
		
		CAIScriptObj * script = (CAIScriptObj *)m_pStatus->m_scrList->GetAt(posIndex);
		script->SafeDelete();
		delete script;
		script=0;
		m_pStatus->m_scrList->RemoveAt(posIndex);
		ScriptInList(scrIndex-1);
	}
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CAISystemDlg::OnBTNAIEditEvent() 
{
#ifndef _ClientOutput
	int entIndex = this->SendDlgItemMessage(IDC_LIST_AI_Scripts,LB_GETCURSEL ,0,0);
	if(entIndex == -1){
	    MessageBox("Must select a script");
		return;
	}
    int eventIndex = this->SendDlgItemMessage(IDC_LIST_AI_Events,LB_GETCURSEL ,0,0);
	if(eventIndex == -1){
	    MessageBox("Must select a event");
		return;
	}


   
	CScriptEventAdd dlg;
       POSITION posIndex = m_pStatus->m_scrList->FindIndex(entIndex);
	   if(posIndex){
          CAIScriptObj * script = (CAIScriptObj *)m_pStatus->m_scrList->GetAt(posIndex);
          if(!script->m_events)
			  return;
			  POSITION eventPos = script->m_events->FindIndex(eventIndex);
		      if(eventPos){
			     CAIScriptEvent * scEvent = (CAIScriptEvent *)script->m_events->GetAt(eventPos);
			     CScriptEventAdd dlg;
				 dlg.m_finalEventInt = scEvent->m_event;
				 dlg.m_miscInt       = scEvent->m_miscInt;
				 if(dlg.DoModal()==IDOK){//1
                    scEvent->m_event         = dlg.m_finalEventInt;
				    scEvent->m_miscInt       = dlg.m_miscInt;
					ScriptEventsInList(eventIndex);
				 }//end 1
			  }
	   }
#endif
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CAISystemDlg::OnBTNAIDeleteEvent() 
{
#ifndef _ClientOutput
	int entIndex = this->SendDlgItemMessage(IDC_LIST_AI_Scripts,LB_GETCURSEL ,0,0);
	if(entIndex == -1)
	{
		MessageBox("Must select a script");
		return;
	}
	int eventIndex = this->SendDlgItemMessage(IDC_LIST_AI_Events,LB_GETCURSEL ,0,0);
	if(eventIndex == -1)
	{
		MessageBox("Must select a event");
		return;
	}
	CScriptEventAdd dlg;
	POSITION posIndex = m_pStatus->m_scrList->FindIndex(entIndex);
	if(posIndex)
	{
		CAIScriptObj * script = (CAIScriptObj *)m_pStatus->m_scrList->GetAt(posIndex);
		if(!script->m_events)
			return;
		POSITION eventPos = script->m_events->FindIndex(eventIndex);
		if(eventPos)
		{
			if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
				return;
			
			CAIScriptEvent * scEvent = (CAIScriptEvent *)script->m_events->GetAt(eventPos);
			delete scEvent;
			scEvent=0;
			script->m_events->RemoveAt(eventPos);
			ScriptEventsInList(eventIndex-1);
		}
	}
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAISystemDlg::OnBTNAICreateNew() 
{
#ifndef _ClientOutput
	  CAiSettingsDlg dlg;
	  dlg.m_gStatusMem				= m_pStatus;
	  dlg.m_edbRef					= m_pStatus->m_entObjList; 
	  dlg.m_refList					= m_pStatus->m_scrList;
	  dlg.m_finalAiScriptAssign		= -1;
	  CAIObj * ai_ptr				= new CAIObj();
	  dlg.m_aiPtrRef				= ai_ptr;  
	  if(dlg.DoModal()==IDOK)
	  {
	  ai_ptr = dlg.m_aiPtrRef;
	  ai_ptr->m_spawnScript			= dlg.m_finalAiScriptAssign;
	  m_pStatus->m_aiList->AddTail(ai_ptr);
	  m_pStatus->m_aiList->ResetIndexReferences();
	  ai_ptr->List_targetFrame		= new CFrameObj(NULL);
	  AI_DBInList(m_pStatus->m_aiList->GetCount()-1);
	  }
#endif
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CAISystemDlg::AITreeInList(int sel)
{
	//
	this->SendDlgItemMessage(IDC_LIST_AI_WebTrees,LB_RESETCONTENT,0,0);
	for(POSITION posLoc = m_pStatus->GL_aiWebDatabase->GetHeadPosition();posLoc != NULL;)
	{
        CAIWebObj * webObj = (CAIWebObj *)m_pStatus->GL_aiWebDatabase->GetNext(posLoc);
        this->SendDlgItemMessage(IDC_LIST_AI_WebTrees,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)webObj->m_name);
	}
	if(sel != -1)
	   this->SendDlgItemMessage(IDC_LIST_AI_WebTrees,LB_SETCURSEL ,sel,0);
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CAISystemDlg::OnBTNAIAddTree() 
{
	//use mapsmodels folder for organization  
	char buffer[_MAX_PATH];
	_getcwd( buffer, _MAX_PATH );
    CString CurDir;
    CString MainDir;	
	MainDir = CurDir = buffer;
	CurDir = CurDir+"\\GameFiles\\";
	SetCurrentDirectory(CurDir);
	CString fileName = "<none>";
	//end use mapsmodels folder for organization
	static char BASED_CODE filter[] = "TRE Files|*.tre";
	CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
    if ( opendialog.DoModal() == IDOK )
		fileName = opendialog.GetPathName();
	else{
		SetCurrentDirectory(MainDir);
		return;
	}
	SetCurrentDirectory(MainDir);
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 CArchive the_in_Archive(&fl,CArchive::load);
	 CAIWebObj * webObj = NULL;
	 the_in_Archive >> webObj;
	 the_in_Archive.Close();
     fl.Close();
	 m_pStatus->GL_aiWebDatabase->AddTail(webObj);
	 AITreeInList(m_pStatus->GL_aiWebDatabase->GetCount()-1);
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CAISystemDlg::OnBTNAIDeleteTree() 
{
	int treeIndex = this->SendDlgItemMessage(IDC_LIST_AI_WebTrees,LB_GETCURSEL ,0,0);
	if(treeIndex == -1)
	{
		MessageBox("Must select a tree");
		return;
	}
	POSITION posIndex = m_pStatus->GL_aiWebDatabase->FindIndex(treeIndex);
	if(posIndex)
	{
		if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
			return;
		
		CAIWebObj * webObj = (CAIWebObj *)m_pStatus->GL_aiWebDatabase->GetAt(posIndex);
		webObj->SafeDelete();
		delete webObj;
		webObj=0;
		m_pStatus->GL_aiWebDatabase->RemoveAt(posIndex);
		AITreeInList(m_pStatus->GL_aiWebDatabase->GetCount()-1);
	}
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CAISystemDlg::OnBTNAIEdit() 
{
#ifndef _ClientOutput
	int aiIndex = this->SendDlgItemMessage(IDC_LIST_AI_DB,LB_GETCURSEL ,0,0);
	if(aiIndex == -1){
	    MessageBox("Must select a bot");
		return;
	}

    POSITION posLoc = m_pStatus->m_aiList->FindIndex(aiIndex);
	if(!posLoc)
		return;
	CAIObj * aiPtr = (CAIObj *) m_pStatus->m_aiList->GetAt(posLoc);

	CAiSettingsDlg dlg;
	  dlg.m_gStatusMem				= m_pStatus;
	  dlg.m_edbRef					= m_pStatus->m_entObjList; 
	  dlg.m_refList					= m_pStatus->m_scrList;
	  dlg.m_finalAiScriptAssign		= aiPtr->m_spawnScript;
	  dlg.m_aiPtrRef				= aiPtr;  
	  if(dlg.DoModal()==IDOK)
	  {
		  aiPtr						= dlg.m_aiPtrRef;
		  aiPtr->m_spawnScript		= dlg.m_finalAiScriptAssign;
		  AI_DBInList(aiIndex);
	  }
#endif
}

void CAISystemDlg::OnBTNAISaveAiData() 
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization
	static char BASED_CODE filter[] = "AII Files|*.AII|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "AII", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_pStatus->m_aiList << m_pStatus->m_scrList;
       the_out_Archive.Close();
       fl.Close();

	 }

	SetCurrentDirectory(MainDir);
}

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CAISystemDlg::OnBTNAILoadAiData() 
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization

   static char BASED_CODE filter[] = "AII Files|*.AII|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   SetCurrentDirectory(MainDir);
	   m_pStatus->LoadAiData(fileName);
	   AI_DBInList(m_pStatus->m_aiList->GetCount()-1);
	   ScriptInList(m_pStatus->m_scrList->GetCount()-1);
	 }
     SetCurrentDirectory(MainDir);
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CAISystemDlg::OnSelchangeLISTAIScripts() 
{
	this->SendDlgItemMessage(IDC_LIST_AI_Events,LB_RESETCONTENT,0,0);
	int entIndex = this->SendDlgItemMessage(IDC_LIST_AI_Scripts,LB_GETCURSEL ,0,0);
	if(entIndex == -1){
		return;
	}
       POSITION posIndex = m_pStatus->m_scrList->FindIndex(entIndex);
	   if(posIndex){
          CAIScriptObj * script = (CAIScriptObj *)m_pStatus->m_scrList->GetAt(posIndex);
          if(script->m_events)
			  ScriptEventsInList(script->m_events->GetCount()-1);
	   }
}
void CAISystemDlg::OnBTNAIEditAITree() 
{
#ifndef _ClientOutput
    int treeIndex = this->SendDlgItemMessage(IDC_LIST_AI_WebTrees,LB_GETCURSEL ,0,0);
	if(treeIndex == -1){
		MessageBox("Must select a tree");
		return;
	}
	POSITION posIndex = m_pStatus->GL_aiWebDatabase->FindIndex(treeIndex);
	 if(posIndex){
        CAIWebObj * webObj = (CAIWebObj *)m_pStatus->GL_aiWebDatabase->GetAt(posIndex);
	    CAIPathDlg dlg;
        POSITION ePos = m_pStatus->m_entObjRuntime->FindIndex(0);
	    if(ePos)
		{
           CMovementObj * ptrLoc = ( CMovementObj* ) m_pStatus->m_entObjRuntime->GetAt(ePos);
           ptrLoc->List_baseFrame->GetPosition(NULL,&dlg.m_charCurrentPosition);
		}
		else{
           dlg.m_charCurrentPosition.x = 0.0f;
		   dlg.m_charCurrentPosition.y = 0.0f;
		   dlg.m_charCurrentPosition.z = 0.0f;
		}
	    
		dlg.m_aiObject = webObj;
        dlg.DoModal();
		webObj->m_name = dlg.m_webName;
		AITreeInList(treeIndex);
	 }
#endif
}

void CAISystemDlg::OnBTNAILoadAiUnit() 
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization

   static char BASED_CODE filter[] = "AIE Files|*.AIE|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
       CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	   CAIObj * partPtr = NULL;
	   CArchive the_in_Archive(&fl,CArchive::load);
	   the_in_Archive >> partPtr;
	   the_in_Archive.Close();
       fl.Close();
	   m_pStatus->m_aiList->AddTail(partPtr);
	   AI_DBInList(m_pStatus->m_aiList->GetCount()-1);
	   ScriptInList(m_pStatus->m_scrList->GetCount()-1);
	 }
     SetCurrentDirectory(MainDir);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAISystemDlg::OnBTNAISaveAiUnit() 
{
    if(!m_pStatus->m_aiList)
	return;
	
    int runtimeIndex = this->SendDlgItemMessage(IDC_LIST_AI_DB,LB_GETCURSEL ,0,0);
    if(runtimeIndex == -1){
     MessageBox("Must Select a bot");
     return;
	}
    POSITION posLoc = m_pStatus->m_aiList->FindIndex(runtimeIndex);
    if(!posLoc)
       return;
       CAIObj * partPtr = (CAIObj *)m_pStatus->m_aiList->GetAt(posLoc);
	   

	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization
	static char BASED_CODE filter[] = "AIE Files|*.AIE|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "AIE", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << partPtr;
       the_out_Archive.Close();
       fl.Close();

	 }

	SetCurrentDirectory(MainDir);
}

void CAISystemDlg::OnBTNAIImportCollisionSys() 
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization

   static char BASED_CODE filter[] = "CUI Files|*.CUI|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
       CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	   if(res == FALSE){
		  SetCurrentDirectory(MainDir);
	   	  return;
	   }
	   CCollisionObj * cObj = NULL;
	   CArchive the_in_Archive(&fl,CArchive::load);
	   the_in_Archive >> cObj;
	   the_in_Archive.Close();
       fl.Close();
	   m_pStatus->m_collisionDatabase->AddTail(cObj);
	 }
	 InListCollisionDB(m_pStatus->m_collisionDatabase->GetCount()-1);
     SetCurrentDirectory(MainDir);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAISystemDlg::OnBTNAIDeleteSys() 
{
	int runtimeIndex = this->SendDlgItemMessage(IDC_LIST_collisionDatabases,LB_GETCURSEL ,0,0);
	if(runtimeIndex == -1)
	{
		MessageBox("Must select a collision unit...");
		return;
	}

	POSITION posLoc = m_pStatus->m_collisionDatabase->FindIndex(runtimeIndex);
	if(!posLoc)
		return;

	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;

	CCollisionObj * cObj = (CCollisionObj *)m_pStatus->m_collisionDatabase->GetAt(posLoc);
	cObj->SafeDelete();
	delete cObj;
	cObj = 0;
	m_pStatus->m_collisionDatabase->RemoveAt(posLoc);

	InListCollisionDB(runtimeIndex-1);
}

void CAISystemDlg::OnBTNUIAIRainSys() 
{
#ifndef _ClientOutput
	D3DVECTOR optionalInitializer;
	optionalInitializer.x = 0.0f;
	optionalInitializer.y = 0.0f;
	optionalInitializer.z = 0.0f;
   POSITION entPosition = m_pStatus->m_entObjRuntime->FindIndex(0);
   if(entPosition)
   {//valid entity   
	       CMovementObj * ptrLoc = ( CMovementObj* ) m_pStatus->m_entObjRuntime->GetAt(entPosition);
		   ptrLoc->List_baseFrame->GetPosition(NULL,&optionalInitializer);
   }

    CAIRaidDbClass dlg;
	dlg.m_posX = optionalInitializer.x;
	dlg.m_posY = optionalInitializer.y;
	dlg.m_posZ = optionalInitializer.z;
	dlg.m_aiRaidRefList = m_pStatus->m_aiRaidDatabase;
	dlg.DoModal();
	m_pStatus->m_aiRaidDatabase = dlg.m_aiRaidRefList;
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAISystemDlg::OnBTNAIExportCurrentCol() 
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "CUI Files|*.CUI|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "CUI", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_pStatus->m_collisionBase;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);  
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAISystemDlg::OnBTNAISaveColDB() 
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "CRD Files|*.CRD|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "CRD", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_pStatus->m_collisionDatabase;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);  
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAISystemDlg::OnBTNAILoadColDB() 
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization

   static char BASED_CODE filter[] = "CRD Files|*.CRD|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   SetCurrentDirectory(MainDir);
	   m_pStatus->LoadAICollisionDB(fileName);    
	 }
     SetCurrentDirectory(MainDir);
	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAISystemDlg::InListCollisionDB(int sel)
{
	this->SendDlgItemMessage(IDC_LIST_collisionDatabases,LB_RESETCONTENT,0,0);	
	for(POSITION posLoc = m_pStatus->m_collisionDatabase->GetHeadPosition();posLoc != NULL;)
	{
        CCollisionObj * cObj = (CCollisionObj *)m_pStatus->m_collisionDatabase->GetNext(posLoc);
		this->SendDlgItemMessage(IDC_LIST_collisionDatabases,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Collision Object");
	}

    if(sel > -1)
	   this->SendDlgItemMessage(IDC_LIST_collisionDatabases,LB_SETCURSEL ,sel,0);

}

void CAISystemDlg::OnBTNAIDelete() 
{
	if(!m_pStatus->m_aiList)
		return;

	int runtimeIndex = this->SendDlgItemMessage(IDC_LIST_AI_DB,LB_GETCURSEL ,0,0);
	if(runtimeIndex == -1)
	{
		MessageBox("Must Select a bot");
		return;
	}
	
	POSITION posLoc = m_pStatus->m_aiList->FindIndex(runtimeIndex);
	if(!posLoc)
		return;

	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
       return;
	
	CAIObj * partPtr = (CAIObj *)m_pStatus->m_aiList->GetAt(posLoc);
	partPtr->SafeDelete();
	delete partPtr;
	partPtr=0;
	m_pStatus->m_aiList->RemoveAt(posLoc);
	AI_DBInList(runtimeIndex-1);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Pathfinding check version 1-Used in AI--------------------------------//
BOOL CAISystemDlg::AI_Roam(CMovementObj * ptrLoc,CAIObj * AIPtr)
{
	BOOL proceed = FALSE;
	/*if(AIPtr->m_status != 0 && AIPtr->m_getSonarInfo == FALSE)
	   proceed = TRUE;*/
	if(ptrLoc->List_AI_reachedDestination == TRUE)
       proceed = TRUE; 
	
	if(proceed == TRUE){
      //get new location to travel to in radius
	  D3DVECTOR locationBuild = m_pStatus->m_cabLib.GetRandomPointInRadius(AIPtr->List_AreaToRoam,ptrLoc->List_AI_origin,0.0f);
	  //application of data
      ptrLoc->List_AI_reachedDestination = FALSE;
      ptrLoc->List_AIdestination = locationBuild;
	}

return TRUE;
}


//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAISystemDlg::AI_DBInList(int sel)
{
if(!m_pStatus->m_aiList)
	return;
	
int traceIt=0;
    this->SendDlgItemMessage(IDC_LIST_AI_DB,LB_RESETCONTENT,0,0);	
	for(POSITION posLoc = m_pStatus->m_aiList->GetHeadPosition();posLoc != NULL;){
        CAIObj * partPtr = (CAIObj *)m_pStatus->m_aiList->GetNext(posLoc);
        this->SendDlgItemMessage(IDC_LIST_AI_DB,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)operator+(m_pStatus->m_cabLib.ConvToString(traceIt),operator+(" cfg:",partPtr->m_aiCfgName)));
		traceIt++;
	}
	if(sel > -1)
	   this->SendDlgItemMessage(IDC_LIST_AI_DB,LB_SETCURSEL ,sel,0);
}
									 

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------------Attack check version 1-Used in AI--------------------------------//
BOOL CAISystemDlg::AI_UseWeapon(CMovementObj * ptrLoc,CAIObj * AIPtr)
{
 
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Create AI Object------------------------------------------------------//
void CAISystemDlg::AddAIObject(float     closeEnoughDist,
							 float     turnTolerance,
							 int       sightType,
		                     int       sightRandVariable,
		                     float     maxSightRadius,
		                     int       difficulty,
		                     BOOL      walkingType,
		                     int       startAttackZone,
		                     int       startAttackDist,
                             int       randChillTime,
		                     int       pathfindingMethod,
		                     int       randKamakazi)
{

CAIObj* AIptr = new CAIObj();
//d3drm3->CreateFrame( scene, &AIptr->List_targetFrame);//build temp frame
AIptr->List_targetFrame = new CFrameObj(NULL);
m_pStatus->m_aiList->AddTail(AIptr);
}


void CAISystemDlg::OnBTNAIExportToSql()
{
     CSqlConnectionDlg dlg;
     if(dlg.DoModal() == IDOK)
	 {
        if(!m_pStatus->m_aiList->ExportToSQL(dlg.m_ipAddress, dlg.m_database, dlg.m_username, dlg.m_password))
			AfxMessageBox("Connection Failed");
		else
		    AfxMessageBox("Completed");
	 }
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAISystemDlg::OnBTNAIIntegrityCheck() 
{
#ifndef _ClientOutput
    //output AII system to sql
    

	//check web boundries
	if(m_pStatus->GL_aiWebDatabase)
	for(POSITION posLoc = m_pStatus->GL_aiWebDatabase->GetHeadPosition();posLoc != NULL;)
	{
        CAIWebObj * web = (CAIWebObj *)m_pStatus->GL_aiWebDatabase->GetNext(posLoc);
        for(POSITION wPos = web->m_web->GetHeadPosition();wPos != NULL;)
		{
            CAIWebNode * nodePtr = (CAIWebNode *) web->m_web->GetNext(wPos);

            if(fabs(nodePtr->m_position.x) > 29000)
			   MessageBox(operator+("Out of range: ",web->m_name));

			if(fabs(nodePtr->m_position.y) > 29000)
			   MessageBox(operator+("Out of range: ",web->m_name));

			if(fabs(nodePtr->m_position.z) > 29000)
			   MessageBox(operator+("Out of range: ",web->m_name));


		}
	}
	AfxMessageBox("Beginning DIM cfg check");
	AIdimIntegrityCheck();
	AfxMessageBox("Completed Bounds tree check");
	AfxMessageBox("Begginning item integrity check");
	//check inventory validity
	int lastSelectedPersist = 0;

	if(m_pStatus->m_aiList)
       m_pStatus->m_aiList->RefreshData(m_pStatus->m_itemManager);

    if(m_pStatus->m_aiList)
    for(POSITION posLoc = m_pStatus->m_aiList->GetHeadPosition();posLoc != NULL;){
        CAIObj * partPtr = (CAIObj *)m_pStatus->m_aiList->GetNext(posLoc);
        
        if(!partPtr->m_inventoryCfg)
			continue;

		POSITION rPosLast;
		for(POSITION rPos = partPtr->m_inventoryCfg->GetHeadPosition();(rPosLast = rPos) != NULL;)
		{//1
            CInventoryObj * invPtr = (CInventoryObj *)partPtr->m_inventoryCfg->GetNext(rPos);

            CItemObj * glidPtr = m_pStatus->m_itemManager->GetItemByID(invPtr->m_itemData->m_globalID);

            int glidOld = invPtr->m_itemData->m_globalID;

            BOOL callRepairMan = FALSE;
            if(!glidPtr)
			    callRepairMan = TRUE;
		    else if(glidPtr->m_itemName != invPtr->m_itemData->m_itemName)
			    callRepairMan = TRUE;
            
		    if(callRepairMan)
			{//r

		    CItemConflictDlg cnDlg;
	        cnDlg.m_lastSelected			= lastSelectedPersist;
	        cnDlg.m_currentInventory		= partPtr->m_inventoryCfg;
	        cnDlg.m_currentInvItem			= invPtr;
	        cnDlg.m_currentItemGlid			= invPtr->m_itemData->m_globalID;
	        cnDlg.m_gStatusMem				= m_pStatus;
	        cnDlg.m_currentName				= invPtr->m_itemData->m_itemName;
	        cnDlg.m_reference				= partPtr->m_nameOvveride;
            int result = cnDlg.DoModal();
			lastSelectedPersist = cnDlg.m_lastSelected;
	        if(cnDlg.m_returnCommand == 5)
		       return;
			if(cnDlg.m_returnCommand == 0)
			{//delete current
               invPtr->SafeDelete();
			   delete invPtr;
			   invPtr = 0;
               partPtr->m_inventoryCfg->RemoveAt(rPosLast);
			}//end delete current
            if(cnDlg.m_returnCommand == 10)
			{
               //SetAllItemData(invPtr->m_itemData,glidOld);
			}
			}//end r

		}//end 1
	}
	//
	Sleep(4000);
	AfxMessageBox("Begginning armed item confict checks");
	if(m_pStatus->m_aiList)
    for(POSITION posLoc = m_pStatus->m_aiList->GetHeadPosition();posLoc != NULL;){
        CAIObj * partPtr = (CAIObj *)m_pStatus->m_aiList->GetNext(posLoc);
        
        if(!partPtr->m_inventoryCfg)
			continue;

		POSITION rPosLast;
		for(POSITION rPos = partPtr->m_inventoryCfg->GetHeadPosition();(rPosLast = rPos) != NULL;)
		{//1
            CInventoryObj * invPtr = (CInventoryObj *)partPtr->m_inventoryCfg->GetNext(rPos);
			if(!invPtr->m_armed)
			   continue;
            for(POSITION rPos2 = partPtr->m_inventoryCfg->GetHeadPosition();rPos2 != NULL;)
			{//2
                CInventoryObj * invPtr2 = (CInventoryObj *)partPtr->m_inventoryCfg->GetNext(rPos2);
				if(!invPtr2->m_armed)
					continue;

				if(invPtr == invPtr2)
				    continue;

				if(invPtr2->m_itemData->m_locationID == invPtr->m_itemData->m_locationID)
				{//conflict exists
					CString build = "armed location ID conflict:";
                    build = build+invPtr2->m_itemData->m_itemName;
					build = build+"  ";
                    build = build+invPtr->m_itemData->m_itemName;
					AfxMessageBox(build);
				}//end conflict exists
			}//end 2
		}//end 1
	}
	Sleep(4000);
	AfxMessageBox("Begginning weapon armed checks-will notify if he has no attack");
	int tracerItTemp = 0;
	if(m_pStatus->m_aiList)
    for(POSITION posLoc = m_pStatus->m_aiList->GetHeadPosition();posLoc != NULL;){
        CAIObj * partPtr = (CAIObj *)m_pStatus->m_aiList->GetNext(posLoc);
        tracerItTemp++;
        if(!partPtr->m_inventoryCfg)
			continue;

		BOOL isArmed = FALSE;
		POSITION rPosLast;
		for(POSITION rPos = partPtr->m_inventoryCfg->GetHeadPosition();(rPosLast = rPos) != NULL;)
		{//1
            CInventoryObj * invPtr = (CInventoryObj *)partPtr->m_inventoryCfg->GetNext(rPos);
			if(!invPtr->m_armed)
			   continue;

			if(invPtr->m_itemData->m_locationID == 12)
				isArmed = TRUE;
            
		}//end 1
		if(!isArmed)
			AfxMessageBox(operator+("This ai index has no armed attack item on id 12:",m_pStatus->m_cabLib.ConvToString(tracerItTemp-1)));
	}

	Sleep(4000);
	AfxMessageBox("Completed");
#endif
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CAISystemDlg::OnBTNAInewTree() 
{
     CAIWebObj * webObj = new CAIWebObj();
	 webObj->m_web = new CAIWebNodeList();
	 m_pStatus->GL_aiWebDatabase->AddTail(webObj);
	 AITreeInList(m_pStatus->GL_aiWebDatabase->GetCount()-1);
}

void CAISystemDlg::OnSelchangeLISTAItree()
{
    int treeIndex = this->SendDlgItemMessage(IDC_LIST_AI_WebTrees,LB_GETCURSEL ,0,0);
	if(treeIndex == -1)
		return;
	POSITION posIndex = m_pStatus->GL_aiWebDatabase->FindIndex(treeIndex);
	if(!posIndex)
	    return;
    CAIWebObj * webObj = (CAIWebObj *)m_pStatus->GL_aiWebDatabase->GetAt(posIndex);
	if(!webObj->m_web)
		return;

	m_pStatus->m_dVectors.Clear();

	for(POSITION posLoc = webObj->m_web->GetHeadPosition();posLoc != NULL;)
	{
        CAIWebNode * wPtr = (CAIWebNode *)webObj->m_web->GetNext(posLoc);
		if(wPtr->m_directLinks)
		for(int iLoop=0;iLoop<wPtr->m_directLinkCount;iLoop++)
		{
			CAIWebNode * wPtrTarget = webObj->m_web->GetByIndex(wPtr->m_directLinks[iLoop]);
			if(wPtrTarget)
			   m_pStatus->m_dVectors.AddLineWLD(wPtr->m_position,wPtrTarget->m_position);
		}
	}
}

void CAISystemDlg::AIdimIntegrityCheck()
{
  for(POSITION posLoc = m_pStatus->m_aiList->GetHeadPosition();posLoc != NULL;)
  {
    CAIObj * partPtr = (CAIObj *)m_pStatus->m_aiList->GetNext(posLoc);
	
    CMovementObj * DBPtr = m_pStatus->m_entObjList->GetObjByIndex(partPtr->m_entityCfgDBindexRef);   
    if(!DBPtr)
	{
        AfxMessageBox(partPtr->m_aiCfgName + " invalid edb index");
		continue;
	}

	if(DBPtr->List_skeletalObject->m_deformableMeshArrayCount == 0)
	   continue;

	if(partPtr->m_spawnCfgCount != DBPtr->List_skeletalObject->m_deformableMeshArrayCount) 
    {//Update dim cfg arrays if damaged
	   m_pStatus->m_multiplayerObject->m_eventLog->AddEvent("CFG Mismatch Upgrading. Correcting: "+partPtr->m_aiCfgName);
       int oldCount = partPtr->m_spawnCfgCount;
	   if(partPtr->m_spawnCfgCount > 0 && oldCount > 0){     
	      partPtr->m_spawnCfgCount = DBPtr->List_skeletalObject->m_deformableMeshArrayCount;
          int cfgSelect = partPtr->m_spawnCfg[0];
		  if(partPtr->m_spawnCfg)
		     delete [] partPtr->m_spawnCfg;
			 partPtr->m_spawnCfg = 0;
					  
		     partPtr->m_spawnCfg = new int[partPtr->m_spawnCfgCount];
		     for(int tempLoop=0;tempLoop<partPtr->m_spawnCfgCount;tempLoop++)
			 {
				 partPtr->m_spawnCfg[tempLoop] = cfgSelect;
			 }			
	   }
    }//end Update dim cfg arrays if damaged
   }
}