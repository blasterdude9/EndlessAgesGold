// LoadScriptsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "direct.h"
#include "CBaseInterfaceModule.h"
#include "LoadScriptsDlg.h"
#include ".\loadscriptsdlg.h"
#include "direct.h"


// CLoadScriptsDlg dialog

IMPLEMENT_DYNAMIC(CLoadScriptsDlg, CDialog)
CLoadScriptsDlg::CLoadScriptsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadScriptsDlg::IDD, pParent)
	, m_miscInt(0)
	, m_miscString(_T(""))
{
	m_scriptListRef       = NULL;
	m_scriptListMemoryRef = NULL;
	m_fromFile            = TRUE;
	m_sceneBased          = FALSE;
	m_currentEdit         = NULL;
}

CLoadScriptsDlg::~CLoadScriptsDlg()
{
	// file mode: the list is ours.  scene mode: it belongs to the engine.
	if(!m_scriptListMemoryRef && m_scriptListRef)
	{
		m_scriptListRef->SafeDelete();
		delete m_scriptListRef;
	}
	m_scriptListRef = NULL;
}

void CLoadScriptsDlg::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete this;   // created with new + Create() (modeless) by CMenuAccessInterface
}

void CLoadScriptsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_scriptsList, CScriptsList);
	DDX_Control(pDX, IDC_COMBO_TypeList, m_typeList);
	DDX_Text(pDX, IDC_EDIT1, m_miscInt);
	DDX_Text(pDX, IDC_EDIT2, m_miscString);
}


BEGIN_MESSAGE_MAP(CLoadScriptsDlg, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_scriptsList, OnLvnItemchangedListscriptslist)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST_scriptsList, OnNMRdblclkListscriptslist)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_scriptsList, OnNMDblclkListscriptslist)
	ON_BN_CLICKED(IDC_BUTTON_AddScriptEvent, OnBnClickedButtonAddscriptevent)
	ON_BN_CLICKED(IDC_BUTTON_DeleteScriptEvent, OnBnClickedButtonDeletescriptevent)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceScriptEvent, OnBnClickedButtonReplacescriptevent)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_LoadFromFile, &CLoadScriptsDlg::OnBnClickedBtnLoadfromfile)
	ON_BN_CLICKED(IDC_BTN_SaveToFile, &CLoadScriptsDlg::OnBnClickedBtnSavetofile)
END_MESSAGE_MAP()


// ---------------------------------------------------------------- list access

// The list this dialog edits.  Never returns NULL.
CEXScriptObjList * CLoadScriptsDlg::List()
{
	if(m_scriptListMemoryRef)
	{
		// scene mode: always follow the engine's live pointer (it may have been
		// freed/re-created since this modeless dialog was opened)
		if(!*m_scriptListMemoryRef)
			*m_scriptListMemoryRef = new CEXScriptObjList();
		m_scriptListRef = *m_scriptListMemoryRef;
	}
	else if(!m_scriptListRef)
	{
		m_scriptListRef = new CEXScriptObjList();
	}
	return m_scriptListRef;
}

CString CLoadScriptsDlg::DefaultScriptPath()
{
	char buffer[_MAX_PATH];
	_getcwd( buffer, _MAX_PATH );
	CString path = buffer;
	if(path.Right(1) != "\\")
		path += "\\";
	return path + "GameFiles\\autoexec.aut";
}

// Reads a CEXScriptObjList from an .aut file.  Returns NULL (and tells the
// user why) if the file is missing, empty or not a script archive.
CEXScriptObjList * CLoadScriptsDlg::LoadListFromFile(const CString & path)
{
	// The script classes (CEXScriptObjList/CEXScriptObj) live in AuraXDynamic.dll and are
	// registered in the APPLICATION module state. This dialog is created under
	// AFX_MANAGE_STATE(AfxGetStaticModuleState()), where CArchive cannot find them
	// ("unexpected class"), so switch to the app state for the archive work.
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	CFileException exc;
	CFile fl;
	if(!fl.Open(path, CFile::modeRead | CFile::shareDenyWrite, &exc))
		return NULL;

	CEXScriptObjList * loaded = NULL;
	try
	{
		CArchive ar(&fl, CArchive::load);
		ar >> loaded;
		ar.Close();
		fl.Close();
	}
	catch(CException * e)
	{
		char msg[512] = "";
		e->GetErrorMessage(msg, sizeof(msg));
		CString detail; detail.Format("%s (%s", msg, e->GetRuntimeClass()->m_lpszClassName);
		if(e->IsKindOf(RUNTIME_CLASS(CArchiveException))) { CString c; c.Format(", cause %d", ((CArchiveException*)e)->m_cause); detail += c; }
		detail += ")";
		e->Delete();
		AfxMessageBox("Could not read script file:\n" + path + "\n\n" + detail, MB_ICONERROR, 0);
		if(fl.m_hFile != CFile::hFileNull)
			fl.Close();
		return NULL;
	}
	return loaded;
}

BOOL CLoadScriptsDlg::SaveListToFile(const CString & path)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	CFileException exc;
	CFile fl;
	if(!fl.Open(path, CFile::modeCreate | CFile::modeWrite, &exc))
	{
		AfxMessageBox("Could not write " + path, MB_ICONERROR, 0);
		return FALSE;
	}
	try
	{
		CArchive ar(&fl, CArchive::store);
		CEXScriptObjList * list = List();
		ar << list;
		ar.Close();
		fl.Close();
	}
	catch(CException * e)
	{
		char msg[512] = "";
		e->GetErrorMessage(msg, sizeof(msg));
		e->Delete();
		AfxMessageBox("Could not write " + path + "\n\n" + msg, MB_ICONERROR, 0);
		return FALSE;
	}
	return TRUE;
}

// Moves the entries of 'loaded' into the list we edit (keeping the engine's
// list object alive in scene mode) and frees 'loaded'.
void CLoadScriptsDlg::ReplaceListContents(CEXScriptObjList * loaded)
{
	CEXScriptObjList * target = List();
	if(loaded == target)
		return;
	target->SafeDelete();
	if(loaded)
	{
		for(POSITION pos = loaded->GetHeadPosition(); pos != NULL;)
			target->AddTail(loaded->GetNext(pos));
		loaded->RemoveAll();   // nodes only - the objects now belong to target
		delete loaded;
	}
}

void CLoadScriptsDlg::UpdateTitle()
{
	CString title = m_scriptListMemoryRef ? "Scene Scripts" : "Load Scripts";
	if(!m_filePath.IsEmpty())
		title += " - " + m_filePath;
	SetWindowText(title);
}


// CLoadScriptsDlg message handlers

BOOL CLoadScriptsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(TRUE);

	if(m_fromFile && !m_scriptListMemoryRef)
	{//load from GameFiles\autoexec.aut
		m_filePath = DefaultScriptPath();
		CEXScriptObjList * loaded = LoadListFromFile(m_filePath);
		if(loaded)
		{
			if(m_scriptListRef)
			{
				m_scriptListRef->SafeDelete();
				delete m_scriptListRef;
			}
			m_scriptListRef = loaded;
		}
	}//end load from autoexec
	List();   // make sure there is always a list to edit


	CScriptsList.InsertColumn(0,"Action",LVCFMT_LEFT,200,0);
	CScriptsList.InsertColumn(1,"Peram Misc Int",LVCFMT_LEFT,30,0);
	CScriptsList.InsertColumn(2,"Peram Misc String",LVCFMT_LEFT,200,0);

	UpdateData(FALSE);

	InListScripts(-1);
	InListEvents(0);
	UpdateTitle();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLoadScriptsDlg::InListScripts(int sel)
{
     int trace = 0;
	 CScriptsList.DeleteAllItems();
	 CEXScriptObjList * list = List();
     for(POSITION posLoc = list->GetHeadPosition();posLoc != NULL;)
	 {
         CEXScriptObj * scrPtr = (CEXScriptObj *) list->GetNext(posLoc);
		 if(!scrPtr)
			 continue;
		 CString attribDesc = GetAttribNameByType(scrPtr->List_actionAttribute);
		 CScriptsList.InsertItem(trace,attribDesc);
		 CScriptsList.SetItemText(trace,1,m_cabLib.ConvToString(scrPtr->List_miscInt));
		 CScriptsList.SetItemText(trace,2,scrPtr->List_miscString);

         trace++;
	 }
	 if(sel > -1 && sel < trace)
	 {
	    CScriptsList.SetItemState(sel,LVIS_SELECTED | LVIS_FOCUSED,LVIS_SELECTED | LVIS_FOCUSED );
		CScriptsList.EnsureVisible(sel,FALSE);
	 }

}

void CLoadScriptsDlg::OnLvnItemchangedListscriptslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	int listSel = pNMLV->iItem;
	if(listSel < 0)
		return;
	if(!(pNMLV->uNewState & LVIS_SELECTED))
		return;   // only react to the item becoming selected

	POSITION posLoc = List()->FindIndex(listSel);
	if(!posLoc)
	    return;

    CEXScriptObj * script = (CEXScriptObj *)List()->GetAt(posLoc);
	if(!script)
		return;
	UpdateData(TRUE);
	m_typeList.SetCurSel(script->List_actionAttribute);
	m_miscInt							= script->List_miscInt;
	m_miscString						= script->List_miscString;
	UpdateData(FALSE);
}


CString CLoadScriptsDlg::GetAttribNameByType(int attribute)
{
	switch(attribute)
	{
		case 0:	 return "Play Fullscreen Movie";
		case 1:	 return "Open Scene";
		case 2:	 return "Load Missile DB";
		case 3:	 return "Load Explosion DB";
		case 4:	 return "Load Runtime Movie DB";
		case 5:	 return "Open Controls Cfg List";
		case 6:	 return "Load Cameras and Viewports";
		case 7:	 return "Load Animated Texture DB";
		case 8:	 return "Init Mouse Object";
		case 9:	 return "Load 2D Configurations";
		case 10: return "Load Text Font Interface";
		case 11: return "Load Sound DB";
		case 12: return "Full Screen Initial Load";
		case 13: return "UNUSED";
		case 14: return "Load Entity Database";
		case 15: return "Load Mouse Database";
		case 16: return "Show Mouse";
		case 17: return "Play Runtime Movie";
		case 18: return "Activate 2d Display Collection";
		case 19: return "Set All Vieports to Focus";
		case 20: return "Set Init TCP/IP Game Default Dialogs";
		case 21: return "Load Hud Database";
		case 22: return "Load Menu Database";
		case 23: return "Open menu";
		case 24: return "Load Particle Systems";
		case 25: return "Load texture databank";
		case 26: return "Load SoundTrack";
		case 27: return "Load AI cfg";
		case 28: return "Load Global Inventory";
		case 29: return "Load Commerce Database";
		case 30: return "Load Bank Zones";
		case 31: return "Load Safe Zones";
		case 32: return "Load Item Generator";
		case 33: return "Load Static Texture Database";
		case 34: return "Load Water Zones DB";
		case 35: return "Load Portal Database";
		case 36: return "Load Element Database";
		case 37: return "Load Stats Database";
		case 38: return "Load Skills Database";
		case 39: return "Load AI Collision Database";
		case 40: return "Load Icon Database";
		case 41: return "Load House Zones";
		case 42: return "Load House DB";
		case 43: return "Load AI Raid DB";
		case 44: return "Load Comm Controls";
		case 45: return "Load Arena Sys";
		case 46: return "Load World Rules";
		case 47: return "Set Username";
		case 48: return "Set Password";
		case 49: return "Set AIserverType Flags";
		case 50: return "Set Connect Join";
		case 51: return "Trigger Spawngen";
		case 52: return "Load shader lib";
		case 53: return "Load Global Shader Lib";
		case 54: return "(OBSOLETE)Load Global Attachment DB";
		case 55: return "Load Pixel Shader Lib";
		case 56: return "Load Post Effects DB";
		case 57: return "Load Temp Entities";
		case 58: return "Load Temp Items";
		case 59: return "Play Music by index";
		case 60: return "Load Selection Effects DB";
		case 61: return "Load Item Manager";
	}

    return "UNKNOWN TYPE";
}


void CLoadScriptsDlg::InListEvents(int sel)
{
     m_typeList.ResetContent();
	 int trace = 0;
     while(GetAttribNameByType(trace)!="UNKNOWN TYPE")
	 {
         m_typeList.AddString(GetAttribNameByType(trace));
		 trace++;
	 }
	 if(sel > -1)
	    m_typeList.SetCurSel(sel);

}

void CLoadScriptsDlg::OnNMRdblclkListscriptslist(NMHDR *pNMHDR, LRESULT *pResult)
{


	*pResult = 0;
}

void CLoadScriptsDlg::OnNMDblclkListscriptslist(NMHDR *pNMHDR, LRESULT *pResult)
{

	*pResult = 0;
}

void CLoadScriptsDlg::OnBnClickedButtonAddscriptevent()
{
	int typeListSelection = m_typeList.GetCurSel();
	if(typeListSelection < 0)
	{
        AfxMessageBox("Must select a  type");
		return;
	}

    UpdateData(TRUE);
	CEXScriptObj * scrPtr = new CEXScriptObj(typeListSelection,m_miscString,m_miscInt);
	List()->AddTail(scrPtr);
    UpdateData(FALSE);
	InListScripts(List()->GetCount()-1);
}

void CLoadScriptsDlg::OnBnClickedButtonDeletescriptevent()
{
	int listSel = CScriptsList.GetSelectionMark();
	if(listSel < 0)
	{
        AfxMessageBox("Must select a  type");
		return;
	}

	POSITION posLoc = List()->FindIndex(listSel);
	if(!posLoc)
	    return;

    CEXScriptObj * script = (CEXScriptObj *)List()->GetAt(posLoc);
	delete script;
	script = 0;
	List()->RemoveAt(posLoc);
	InListScripts(listSel-1);
}

void CLoadScriptsDlg::OnBnClickedButtonReplacescriptevent()
{
	int listSel = CScriptsList.GetSelectionMark();
	if(listSel < 0)
	{
        AfxMessageBox("Must select a  type");
		return;
	}

	int typeListSelection = m_typeList.GetCurSel();
	if(typeListSelection < 0)
	{
        AfxMessageBox("Must select a  type");
		return;
	}

	POSITION posLoc = List()->FindIndex(listSel);
	if(!posLoc)
	    return;

    CEXScriptObj * script = (CEXScriptObj *)List()->GetAt(posLoc);
	if(!script)
		return;
	UpdateData(TRUE);
	script->List_actionAttribute		= typeListSelection;
	script->List_miscInt				= m_miscInt;
	script->List_miscString				= m_miscString;
	UpdateData(FALSE);

	InListScripts(listSel);
}

void CLoadScriptsDlg::OnBnClickedOk()
{
	if(m_fromFile && !m_scriptListMemoryRef)
	{//Save to the exec file this dialog was loaded from
		if(m_filePath.IsEmpty())
			m_filePath = DefaultScriptPath();
		if(SaveListToFile(m_filePath))
			AfxMessageBox("Saved to " + m_filePath,0,0);
		UpdateTitle();
		return;
	}//end save to exec file
	AfxMessageBox("Scene script updated",0,0);
}

void CLoadScriptsDlg::OnBnClickedCancel()
{
	DestroyWindow();   // modeless: EndDialog() is not valid here
}


void CLoadScriptsDlg::OnBnClickedBtnLoadfromfile()
{
	CString MainDir;
    char buffer[_MAX_PATH];
    _getcwd( buffer, _MAX_PATH );
	MainDir = buffer;
	static char BASED_CODE filter[] = "Aut Files|*.aut|All Files (*.*)|*.*||";
	CFileDialog opendialog( TRUE, "aut", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
	CString startDir = MainDir + "\\GameFiles";
	opendialog.m_ofn.lpstrInitialDir = startDir;
    if ( opendialog.DoModal() == IDOK )
	{
		CString path = opendialog.GetPathName();
		SetCurrentDirectory(MainDir);   // the file dialog changes the process cwd
		CEXScriptObjList * loaded = LoadListFromFile(path);
		if(!loaded)
		{
			AfxMessageBox("That script file is empty or could not be read - list unchanged.",MB_ICONWARNING,0);
		}
		else
		{
			ReplaceListContents(loaded);
			m_filePath = path;
		}
		InListScripts(-1);
		UpdateTitle();
	 }
	SetCurrentDirectory(MainDir);
}

void CLoadScriptsDlg::OnBnClickedBtnSavetofile()
{
	 CString MainDir;
     char buffer[_MAX_PATH];
     _getcwd( buffer, _MAX_PATH );
	 MainDir = buffer;
     static char BASED_CODE filter[] = "AUT Files|*.aut|All Files (*.*)|*.*||";
	 CString startName = m_filePath.IsEmpty() ? CString("autoexec.aut") : m_filePath;
     CFileDialog opendialog( FALSE, "aut", startName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString path = opendialog.GetPathName();
	   SetCurrentDirectory(MainDir);
	   if(SaveListToFile(path))
	   {
		   m_filePath = path;
		   AfxMessageBox("Saved to " + path,0,0);
	   }
	   UpdateTitle();
	 }
	 SetCurrentDirectory(MainDir);
}
