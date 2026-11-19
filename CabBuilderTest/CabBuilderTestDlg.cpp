// CabBuilderTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CabBuilderTest.h"
#include "CabBuilderTestDlg.h"
#include "direct.h"
#include ".\cabbuildertestdlg.h"
#include "axfileopen.h"
#include "..\HeaderFiles\CoreStatic\cabinet.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCabBuilderTestDlg dialog



CCabBuilderTestDlg::CCabBuilderTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCabBuilderTestDlg::IDD, pParent)
	, m_baseDirectory(_T(""))
	, m_cabName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCabBuilderTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BaseDirectory, m_baseDirectory);
	DDX_Control(pDX, IDC_LIST_FileList, m_fileList);
	DDX_Control(pDX, IDC_PROGRESS_ONE, m_progressCtrl);
	DDX_Text(pDX, IDC_EDIT_cabName, m_cabName);
}

BEGIN_MESSAGE_MAP(CCabBuilderTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_addFiles, OnBnClickedButtonaddfiles)
	ON_BN_CLICKED(IDC_BUTTON_BaseDirectory, OnBnClickedButtonBasedirectory)
	ON_BN_CLICKED(IDC_BUTTON_compress, OnBnClickedButtoncompress)
	ON_BN_CLICKED(IDC_BUTTON_OpenFiles, OnBnClickedButtonOpenfiles)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_Extract, &CCabBuilderTestDlg::OnBnClickedButtonExtract)
END_MESSAGE_MAP()


// CCabBuilderTestDlg message handlers

BOOL CCabBuilderTestDlg::OnInitDialog()
{
	CoInitialize(NULL);
	CDialog::OnInitDialog();

	UpdateData(TRUE);
	m_cabName = "base";
	UpdateData(FALSE);
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCabBuilderTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCabBuilderTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*void CCabBuilderTestDlg::OnBnClickedButton1()
{
	CCabinetBuilder cb(12345,5000000,5000000,10000000);
	if(cb.InitCabinet("C:\\Temp\\Test","TESTRRR","Disk"))
    {
     // And now we add the files
     if(!cb.AddFile("C:\\EnginePlatform\\ServerMonitor.exe","ServerMonitor.exe",1,1) ||
        !cb.AddFile("C:\\EnginePlatform\\TEST.ONL", "TEST.ONL",1,1) ||
        !cb.AddFile("C:\\EnginePlatform\\Beta10alpha\\AuraSilver9y.exe","Beta10alpha\\AuraSilver9y.exe",1,1 ) ||
        !cb.AddFile("C:\\EnginePlatform\\Beta10alpha\\cards.bmp","Beta10alpha\\cards.bmp",1,1))
	TRACE("Could not add file(s) ([d][%d][%d])\n",
     cb.GetErrorCode(),
     cb.GetErrorCodeEx(),
     errno);


	 cb.WaitUntilDone();
	
	 

	}
    

}*/

void CCabBuilderTestDlg::OnBnClickedButtonaddfiles()
{

	
	//char temp;
    char bigBuff[20000] = "";  // maximum common dialog buffer size
	static char BASED_CODE filter[] = "All Files|*.*|3ds Files|*.3ds|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT , NULL, this );
     opendialog.m_ofn.hInstance = AfxGetInstanceHandle();
	 opendialog.m_ofn.lpstrFile = bigBuff;
	 opendialog.m_ofn.nMaxFile = sizeof(bigBuff);
	  if ( opendialog.DoModal() == IDOK )
	  {//file chose
		  CString FileNames[2000];
	      int counter = 0;
	      int length = 0;
	      int length2 = 0;
		  int loop=0;
	      for(loop=0;loop<15000;loop++){
		    if(bigBuff[loop] == NULL)
			{
			   if(counter > 0)
			   FileNames[counter] = FileNames[0] + "\\" + FileNames[counter];
		       counter++;
		       if(bigBuff[loop+1] == NULL)
               break; 
		       loop++;
			}
		    CString temp = (CString)bigBuff[loop];
		    FileNames[counter] = operator+(FileNames[counter],temp);
		  }
	      if(counter == 1){
             FileNames[1] = opendialog.GetPathName();
	         counter++;
		  }
	      //---------end-build selected files database------------//
		  for(loop=1;loop<counter;loop++)
		  {//begin file load loop
		    
			  if(m_fileList.GetCount()==0)
			  {
			     UpdateData(TRUE);
			     m_baseDirectory = FileNames[0] + "\\";
				 UpdateData(FALSE);
			  }
			  //FileNames[loop]
			  m_fileList.AddString(FileNames[loop]);
		  }
	  }// end file chosen		  
}

void CCabBuilderTestDlg::OnBnClickedButtonBasedirectory()
{
	 static char BASED_CODE filter[] = "All Files|*.*|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   m_baseDirectory = opendialog.GetPathName();
	   UpdateData(FALSE);
	 }
}

void CCabBuilderTestDlg::OnBnClickedButtoncompress()
{
	int fileCount = m_fileList.GetCount();
	m_progressCtrl.SetRange32(0,fileCount-1);
	CCabinetBuilder cb(12345,0,0,0);
	UpdateData(TRUE);
    CString cabName = m_cabName;
	UpdateData(FALSE);
	if(cb.InitCabinet(m_baseDirectory,cabName,"Disk"))
    {
     // And now we add the files
       for(int loop=0;loop<fileCount;loop++)
	   {
		   CString str,location;
		   int n = m_fileList.GetTextLen( loop );
           m_fileList.GetText( loop, str.GetBuffer(n) );
           str.ReleaseBuffer();
           location = str;
           str.Replace(m_baseDirectory,"");

           if(!cb.AddFile(location.GetBuffer(),str.GetBuffer(),1,1))
		   {
			   AfxMessageBox("Failed to add file:"+location +" storedAs:"+str);
		       
		   }
		   m_progressCtrl.SetPos(loop);
	   }
	   
		
		/*if(!cb.AddFile("C:\\EnginePlatform\\ServerMonitor.exe","ServerMonitor.exe",1,1) ||
        !cb.AddFile("C:\\EnginePlatform\\TEST.ONL", "TEST.ONL",1,1) ||
        !cb.AddFile("C:\\EnginePlatform\\Beta10alpha\\AuraSilver9y.exe","Beta10alpha\\AuraSilver9y.exe",1,1 ) ||
        !cb.AddFile("C:\\EnginePlatform\\Beta10alpha\\cards.bmp","Beta10alpha\\cards.bmp",1,1))
	TRACE("Could not add file(s) ([d][%d][%d])\n",
     cb.GetErrorCode(),
     cb.GetErrorCodeEx(),
     errno);*/
	 cb.WaitUntilDone();
	}
}

void CCabBuilderTestDlg::OnBnClickedButtonOpenfiles()
{

	CAXFileOpen dlg;
	if(dlg.DoModal()==IDOK)
	{
		for(int loop=0;loop<dlg.m_fileInfoCount;loop++)
		{
			m_fileList.AddString(dlg.m_fileInfo[loop].s_filePath);
		}
	}


}

void CCabBuilderTestDlg::OnDestroy()
{
	CDialog::OnDestroy();

	CoUninitialize();
}

void CCabBuilderTestDlg::OnBnClickedButtonExtract()
{
	int percentage = 0;
	CResourceCabinet newCabObj;
	newCabObj.Init();
	newCabObj.CreateFDIContext();
    newCabObj.SetProgressCountRef(&percentage);
	newCabObj.SetCriticalSection(NULL);
	
	//newCabObj.Copy(classObj->m_saveLocationWFile.GetBuffer(),classObj->m_installPath,NULL);


	int fileCount = m_fileList.GetCount();

       for(int loop=0;loop<fileCount;loop++)
	   {
		   CString str,location;
		   int n = m_fileList.GetTextLen( loop );
           m_fileList.GetText( loop, str.GetBuffer(n) );
           str.ReleaseBuffer();
           location = str;
           str.Replace(m_baseDirectory,"");

		   newCabObj.Copy(location.GetBuffer(),m_baseDirectory.GetBuffer(),NULL);
           
	   }
	
}
