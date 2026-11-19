//DirectorySelect.cpp : implementation file
//

#include "stdafx.h"
#include "ActiveXPortal.h"
#include "DirectorySelect.h"
#include "directoryselect.h"


//CDirectorySelect dialog

IMPLEMENT_DYNAMIC(CDirectorySelect, CDialog)
CDirectorySelect::CDirectorySelect(CWnd* pParent /*=NULL*/)
	: CDialog(CDirectorySelect::IDD, pParent)
	, m_selectedDirectory(_T(""))
{
}

CDirectorySelect::~CDirectorySelect()
{
}

void CDirectorySelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_directorySelect, m_selectedDirectory);
}


BEGIN_MESSAGE_MAP(CDirectorySelect, CDialog)
	ON_BN_CLICKED(ID_BTN_BROWSE, OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


//CDirectorySelect message handlers

void CDirectorySelect::OnBnClickedBtnBrowse()
{
    /*OPENFILENAME ofn;       //common dialog box structure
	char szFile[260];       //buffer for file name
	HWND hwnd;              //owner window
	HANDLE hf;              //file handle

	//Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	//
	//Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	//use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


    GetFolderName()*/

	//end use mapsmodels folder for organization
	static char BASED_CODE filter[] = "Directory|*.*|Directory|*.*|All Files (*.*)|*.*||";
	CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_ENABLEHOOK | OFN_EXPLORER, filter, this );
	if ( opendialog.DoModal() == IDOK )
	{
		UpdateData(TRUE);
        CString rawPath = opendialog.GetPathName();
		bool write = false;
        for(int pLoop = rawPath.GetLength()-1;pLoop>-1;pLoop--)
		{
            if(rawPath.GetAt(pLoop)=='\\')
			{
			   if(write == false)
			      m_selectedDirectory = "";
			   write = true;
			}
			if(write)
			   m_selectedDirectory = rawPath.GetAt(pLoop) + m_selectedDirectory;
		}

        UpdateData(FALSE);
	}
}

void CDirectorySelect::OnBnClickedOk()
{
	UpdateData(TRUE);
	int length = m_selectedDirectory.GetLength();
	UpdateData(FALSE);
	if(m_selectedDirectory.GetLength() == 0)
	{
       AfxMessageBox("A Path must exist");
	   return;
	}
	UpdateData(TRUE);
    //ensure the directory is capped with 
    if(m_selectedDirectory.GetAt(length-1) != '\\')
	   m_selectedDirectory += '\\';

	UpdateData(FALSE);


	OnOK();
}
