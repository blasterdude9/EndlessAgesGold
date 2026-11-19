// AddDoTdlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "AddDoTdlg.h"
#include "Direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// AddDoTdlg dialog

IMPLEMENT_DYNAMIC(AddDoTdlg, CDialog)

AddDoTdlg::AddDoTdlg(CWnd* pParent /*=NULL*/)
	: CDialog(AddDoTdlg::IDD, pParent)
{
	m_dotName		= _T("");
	m_dotDuration	= 0;
	m_dotAmount		= 0;
	m_dotEffect		= -1;
	m_dotApplyPer	= 0;
	m_dotID			= 0;
}

void AddDoTdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//Exchange update Info
	DDX_Control(pDX, IDC_COMBO_DOT_TYPE, m_dotType);
	DDX_Control(pDX, IDC_COMBO_DOT_CHANNEL, m_dotChannel);
	DDX_Control(pDX, IDC_LIST_DOTS, m_dotList);
	DDX_Text(pDX, IDC_EDIT_DOT_NAME, m_dotName);
	DDX_Text(pDX, IDC_EDIT_DOT_DURATION, m_dotDuration);
	DDX_Text(pDX, IDC_EDIT_DOT_AMOUNT, m_dotAmount);
	DDX_Text(pDX, IDC_EDIT_DOT_EFFECTREF, m_dotEffect);
	DDX_Text(pDX, IDC_EDIT_DOT_ATTACHPER, m_dotApplyPer);
	DDX_Text(pDX, IDC_EDIT_DOT_ID, m_dotID);
}


BEGIN_MESSAGE_MAP(AddDoTdlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_AddDoT, OnBUTTONAddDot)
	ON_BN_CLICKED(IDC_BUTTON_RemoveDoT, OnBUTTONRemoveDot)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceDoT, OnBUTTONReplaceDot)
	ON_BN_CLICKED(IDC_BUTTON_SaveDoT, OnBUTTONSaveDot)
	ON_BN_CLICKED(IDC_BUTTON_LoadDoT, OnBUTTONLoadDot)
	ON_LBN_SELCHANGE(IDC_LIST_DOTS, OnLbnSelchangeDotList)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAddDoTDlg message handlers
BOOL AddDoTdlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);

	if(m_currentElements)
	{
		for(POSITION posLoc = m_currentElements->GetHeadPosition();posLoc != NULL;)
		{
			CElementObj * elementPtr = (CElementObj *) m_currentElements->GetNext(posLoc);
			m_dotChannel.AddString(elementPtr->m_elementName);
		}
	}
	m_dotChannel.SetCurSel(0);
	m_dotType.SetCurSel(0);

	InListDots(0);

	return TRUE;
}
void AddDoTdlg::OnBUTTONAddDot()
{
	CDotClass *newDot = new CDotClass();
	UpdateData(TRUE);

	newDot->m_dotAmount = m_dotAmount;
	newDot->m_dotApplyPer = m_dotApplyPer;
	newDot->m_dotChannel = m_dotChannel.GetCurSel();
	newDot->m_dotDuration = m_dotDuration;
	newDot->m_dotEffectRef = m_dotEffect;
	newDot->m_dotName = m_dotName;
	newDot->m_dotType = m_dotType.GetCurSel();
	newDot->m_dotIndex = 0;
	CDotClass *pPrev = NULL;
	POSITION prevPos = NULL;
	char addToTail = 1;
	for(POSITION posLoc = m_dotDBREF->GetHeadPosition(); posLoc != NULL;)
	{
		prevPos = posLoc;
		CDotClass *dotPtr = (CDotClass *)m_dotDBREF->GetNext(posLoc);
		if(dotPtr->m_dotIndex == 0)
		{
			pPrev = dotPtr;
			continue;
		}

		if(dotPtr->m_dotIndex - pPrev->m_dotIndex > 1)
		{
			newDot->m_dotIndex = pPrev->m_dotIndex + 1;
			m_dotDBREF->InsertBefore(prevPos, newDot);
			addToTail = 0;
			break;
		}
		
		pPrev = dotPtr;
	}

	if(addToTail)
	{
		if(m_dotDBREF->GetCount())
			newDot->m_dotIndex = m_dotDBREF->GetCount();
		m_dotDBREF->AddTail(newDot);
	}
	
	UpdateData(FALSE);
	InListDots(m_dotDBREF->GetCount());
}
void AddDoTdlg::OnBUTTONRemoveDot()
{
	int sel = m_dotList.GetCurSel();
	if(sel == -1)
	{
		AfxMessageBox("Must select a Dot");
		return;
	}

	POSITION posLoc = m_dotDBREF->FindIndex(sel);
	if(posLoc)
	{
		CDotClass *newDot = (CDotClass *) m_dotDBREF->GetAt(posLoc);
		newDot->SafeDelete();
		delete newDot;
		newDot = 0;
		m_dotDBREF->RemoveAt(posLoc);
	}
	InListDots(m_dotDBREF->GetCount());
}
void AddDoTdlg::OnBUTTONReplaceDot()
{
	int dotIndex = m_dotList.GetCurSel();
	if(dotIndex == -1)
	{
		MessageBox("Must select a dot object");
		return;
	}
	POSITION posLoc = m_dotDBREF->FindIndex(dotIndex);
	if(!posLoc)
		return;

	CDotClass *dotPtr = (CDotClass*) m_dotDBREF->GetAt(posLoc);

	dotPtr->m_dotAmount = GetDlgItemInt(IDC_EDIT_DOT_AMOUNT, NULL, TRUE);
	dotPtr->m_dotApplyPer = GetDlgItemInt(IDC_EDIT_DOT_ATTACHPER, NULL, TRUE);
	dotPtr->m_dotDuration = GetDlgItemInt(IDC_EDIT_DOT_DURATION, NULL, TRUE);
	dotPtr->m_dotEffectRef = GetDlgItemInt(IDC_EDIT_DOT_EFFECTREF, NULL, TRUE);
	dotPtr->m_dotIndex = GetDlgItemInt(IDC_EDIT_DOT_ID, NULL, TRUE);
	dotPtr->m_dotType = m_dotType.GetCurSel();
	dotPtr->m_dotChannel = m_dotChannel.GetCurSel();
	GetDlgItemText(IDC_EDIT_DOT_NAME, dotPtr->m_dotName);

	InListDots(dotIndex);
}
void AddDoTdlg::OnBUTTONSaveDot()
{
	//use gamefiles folder for organization
	CString CurDir;
	CString MainDir;
	char buffer[_MAX_PATH];
	_getcwd(buffer, _MAX_PATH);
	MainDir = CurDir = buffer;
	CurDir = operator+(CurDir,"\\GameFiles\\");
	SetCurrentDirectory(CurDir);
	//end use gamefiles folder for orgnization

	static char BASED_CODE filter[] = "DoT Data Files|*.DDB|All Files (*.*)|*.*||";
	CFileDialog opendialog(FALSE, "DDB", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this);
	if(opendialog.DoModal() == IDOK)
	{
		CString fileName = opendialog.GetFileName();
		CFileException exc;
		CFile f1;
		BOOL res = f1.Open(fileName, CFile::modeCreate | CFile::modeWrite, &exc);
		if(res == FALSE)
		{
			SetCurrentDirectory(MainDir);
			return;
		}
		CArchive the_out_Archive(&f1, CArchive::store);
		the_out_Archive << m_dotDBREF;
		the_out_Archive.Close();
		f1.Close();
	}
	SetCurrentDirectory(MainDir);
}
void AddDoTdlg::OnBUTTONLoadDot()
{
	//use gamefiles folder for organization
	CString CurDir;
	CString MainDir;
	char buffer[_MAX_PATH];
	_getcwd(buffer, _MAX_PATH);
	MainDir = CurDir = buffer;
	CurDir = operator+(CurDir,"\\GameFiles\\");
	SetCurrentDirectory(CurDir);
	//end use gamefiles folder for organization

	static char BASED_CODE filter[] = "DoT Data Files|*.DDB|All Files (*.*)|*.*||";
	CFileDialog opendialog(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this);
	if(opendialog.DoModal() == IDOK)
	{
		CString fileName = opendialog.GetPathName();
		SetCurrentDirectory(MainDir);
		CFileException exc;
		CFile f1;
		BOOL res = f1.Open(fileName, CFile::modeRead, &exc);
		if(res == FALSE)
		{
			SetCurrentDirectory(MainDir);
			return;
		}
		CArchive the_in_Archive(&f1, CArchive::load);
		m_dotDBREF->SafeDelete();
		delete m_dotDBREF;
		m_dotDBREF = 0;
		the_in_Archive >> m_dotDBREF;
		the_in_Archive.Close();
		f1.Close();
	}
	SetCurrentDirectory(MainDir);
	InListDots(0);
}
void AddDoTdlg::OnBnClickedOk()
{
	OnCancel();
}
void AddDoTdlg::OnLbnSelchangeDotList()
{
	int dotIndex = m_dotList.GetCurSel();
	if(dotIndex == -1)
	{
		MessageBox("Must select a dot object");
		return;
	}
	POSITION posLoc = m_dotDBREF->FindIndex(dotIndex);
	if(!posLoc)
		return;

	CDotClass *dotPtr = (CDotClass*) m_dotDBREF->GetAt(posLoc);

	SetDlgItemText(IDC_EDIT_DOT_NAME, dotPtr->m_dotName);
	SetDlgItemInt(IDC_EDIT_DOT_AMOUNT, dotPtr->m_dotAmount, TRUE);
	SetDlgItemInt(IDC_EDIT_DOT_DURATION, dotPtr->m_dotDuration, TRUE);
	SetDlgItemInt(IDC_EDIT_DOT_EFFECTREF, dotPtr->m_dotEffectRef, TRUE);
	SetDlgItemInt(IDC_EDIT_DOT_ATTACHPER, dotPtr->m_dotApplyPer, TRUE);
	SetDlgItemInt(IDC_EDIT_DOT_ID, dotPtr->m_dotIndex, TRUE);

	m_dotChannel.SetCurSel(dotPtr->m_dotChannel);
	m_dotType.SetCurSel(dotPtr->m_dotType);
}
void AddDoTdlg::OnOK()
{
}
void AddDoTdlg::InListDots(int sel)
{
	m_dotList.ResetContent();
	for(POSITION posLoc = m_dotDBREF->GetHeadPosition(); posLoc != NULL;)
	{
		CDotClass *dotPtr = (CDotClass *)m_dotDBREF->GetNext(posLoc);
		m_dotList.AddString(dotPtr->m_dotName);
	}
	if(sel > -1)
		m_dotList.SetCurSel(sel);
}