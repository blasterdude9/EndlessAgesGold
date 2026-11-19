// XsrcInstallerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XsrcInstaller.h"
#include "XsrcInstallerDlg.h"
#include "direct.h"

#include "XsrcInstaller.h"
#include "LicenseAgreementDlg.h"
#include "InstallComplete.h"
#include ".\xsrcinstallerdlg.h"
#include "HttpFileDownloader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CXsrcInstallerDlg dialog



CXsrcInstallerDlg::CXsrcInstallerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXsrcInstallerDlg::IDD, pParent)
	, m_installPath(_T(""))
	, m_statusOutput(_T(""))
	, m_kbout(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXsrcInstallerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_Install, m_progressInstall);
	DDX_Text(pDX, IDC_EDIT_installationPath, m_installPath);
	DDX_Control(pDX, IDC_BUTTON_BuildInstaller, m_buildInstallerBtn);
	DDX_Control(pDX, IDC_BUTTON_Install, m_installBtn);
	DDX_Control(pDX, IDC_BUTTON_Uninstall, m_uninstallBtn);
	DDX_Control(pDX, IDC_EDIT_installationPath, m_installPathCnt);
	DDX_Text(pDX, IDC_STATUS_Output, m_statusOutput);
	DDX_Text(pDX, IDC_STATIC_KBOUT, m_kbout);
	DDX_Control(pDX, IDC_STATIC_KBOUT, m_kbOutVis);
}

BEGIN_MESSAGE_MAP(CXsrcInstallerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_Install, OnBnClickedButtonInstall)
	ON_BN_CLICKED(IDC_BUTTON_BuildInstaller, OnBnClickedButtonBuildinstaller)
	ON_BN_CLICKED(IDC_BUTTON_Uninstall, OnBnClickedButtonUninstall)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_installationPath, OnEnChangeEditinstallationpath)
END_MESSAGE_MAP()

DWORD WINAPI InstallIt(LPVOID lpParameter);
//BOOL WINAPI SHCreateShortcut(LPTSTR szShortcut,LPTSTR szTarget );

// CXsrcInstallerDlg message handlers

BOOL CXsrcInstallerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_lastbytesDownloaded = 0;
	m_kbSec = 0;
	m_LastTimeStampBytes = GetTickCount();
	m_bytesDownloaded = 0;
	m_progressInstall.SetRange32(0,100);
	m_globalPercDone = 0;
	m_timerLock = FALSE;
	m_closeNow = FALSE;

	InitializeCriticalSection(&m_cSection);

	m_appTitle = "Endless Ages";

	UpdateData(TRUE);
	this->SetWindowText(m_appTitle + " Installer");
	UpdateData(FALSE);

    #ifdef Installer_R
	    m_installPathCnt.ShowWindow(TRUE);
        m_uninstallBtn.ShowWindow(FALSE);
		m_buildInstallerBtn.ShowWindow(FALSE);
    #endif
	#ifdef Uninstaller_R
		m_installPathCnt.ShowWindow(FALSE);
        m_installBtn.ShowWindow(FALSE);
		m_buildInstallerBtn.ShowWindow(FALSE);
    #endif

	//SHCreateShortcut( _T("\\My Documents\\Windows Media Player.lnk"),_T("\\Windows\\Ceplayer.exe"));
	CoInitialize(NULL);
	UpdateData(TRUE);
	m_installPath = "c:\\Program Files\\"+m_appTitle+"\\";
	UpdateData(FALSE);
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//SetTimer(1,2000,0);


	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CXsrcInstallerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CXsrcInstallerDlg::OnPaint() 
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
HCURSOR CXsrcInstallerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CXsrcInstallerDlg::OnBnClickedButtonInstall()
{

	
	 
	m_installBtn.ShowWindow(FALSE);
	CLicenseAgreementDlg licDlg;
	licDlg.m_licenseEdit = "you must agree with the license posted here http://www.endlessagesportal.com/user.htm before installation.";
    if(licDlg.DoModal() != IDOK)
	{
	   OnOK();
	   return;
	}
	UpdateData(TRUE);
	CString temp = m_installPath;
	UpdateData(FALSE);
	m_installPathCnt.UpdateWindow();
	m_installPathCnt.ShowWindow(FALSE);
	m_kbOutVis.ShowWindow(TRUE);

	//SetCurrentDirectory(m_installPath);


	DWORD m_lgThreadID; 
    m_lgThreadHandle = CreateThread(NULL,0,InstallIt,this,0,&m_lgThreadID);
	SetTimer(1, 200, 0);  

	
    //install test extraction 

	/*CResourceCabinet testCab;
	testCab.Init();
	testCab.CreateFDIContext();
	testCab.SetProgressCountRef(&m_globalPercDone);
	testCab.Copy("C:\\Projects\\XsrcInstaller\\InstallTest\\TestExtract\\setupe.cab","C:\\Projects\\XsrcInstaller\\InstallTest\\TestExtract\\",NULL);
	/*CCabinetObj cabObj;
	CFileException execUninstallDatX;
    CFile uninstallDatFileHandleX;
	CArchive the_out_ArchiveX(&uninstallDatFileHandleX,CArchive::store);
    BOOL uninstallDatResX = uninstallDatFileHandleX.Open("C:\\Projects\\XsrcInstaller\\InstallTest\\TestExtract\\uninstall.idt",CFile::modeCreate | CFile::modeWrite, &execUninstallDatX);

	cabObj.ExtractCabinetToDirectory(   "C:\\Projects\\XsrcInstaller\\InstallTest\\TestExtract\\",//classObj->m_installPath,
									    "C:\\Projects\\XsrcInstaller\\InstallTest\\TestExtract\\setupe.cab",//classObj->m_installPath+fileTragetName,
													&m_globalPercDone,
													&m_cSection,
													the_out_ArchiveX);

	the_out_ArchiveX.Close();
	uninstallDatFileHandleX.Close();*/
	//end test
}

void CXsrcInstallerDlg::OnBnClickedButtonBuildinstaller()
{
    int fileCount = 2;
	UINT totalFileSize = 0;
	CString cabArray[256];
	cabArray[0] = "installer.exe";
	cabArray[1] = "base.CAB";
	int entryPointWritten = 0;
	for(int sizeloop=0;sizeloop<fileCount;sizeloop++)
	{
		CFileException exc;
		CFile fl;
		BOOL res = fl.Open(cabArray[sizeloop],CFile::modeRead, &exc);
		if(!res)
		{
			AfxMessageBox("cannot find file:"+cabArray[sizeloop]);
            return;
		}
		
	    UINT length = fl.GetLength();
	    if(entryPointWritten == 1)
		{
		   totalFileSize += sizeof(SPackageHeader) + cabArray[sizeloop].GetLength();
		}
        totalFileSize += length;
        fl.Close();
		entryPointWritten = 1;
	}
	
    char * writedata = new char[totalFileSize+1];
	ZeroMemory(writedata,totalFileSize+1);
	int tracer = 0;
	entryPointWritten = 0;
    for(int cpyLoop=0;cpyLoop<fileCount;cpyLoop++)
	{
		CFileException exc;
		CFile fl;
		BOOL res = fl.Open(cabArray[cpyLoop],CFile::modeRead, &exc);
		UINT length = fl.GetLength();
        char * thisFileData = new char[length];
        fl.Read(thisFileData,length);
        fl.Close();

		if(entryPointWritten == 1)
		{
			SPackageHeader subHeader;
			subHeader.vDirNameLength = cabArray[cpyLoop].GetLength();
			subHeader.dataBlockLength = length;
			subHeader.vDirName = new char[subHeader.vDirNameLength];
			
			for(int chl=0;chl<subHeader.vDirNameLength;chl++)
				subHeader.vDirName[chl] = cabArray[cpyLoop].GetAt(chl);

			CopyMemory(&writedata[tracer],&subHeader,sizeof(SPackageHeader));
			tracer += sizeof(SPackageHeader);

			CopyMemory(&writedata[tracer],subHeader.vDirName,subHeader.vDirNameLength);
			tracer += subHeader.vDirNameLength;
			delete [] subHeader.vDirName;
		    subHeader.vDirName = 0;
		}

        CopyMemory(&writedata[tracer],thisFileData,length);
        tracer += length;
		delete [] thisFileData;
		thisFileData = 0;
		entryPointWritten = 1;
	}

	//create setup.exe
	CString outputFileName = "Setup.exe";
	CFileException exc;
    CFile fl;
    BOOL res = fl.Open(outputFileName,CFile::modeCreate | CFile::modeWrite, &exc);
	fl.Write(writedata,totalFileSize);
	fl.Close();
	
}

void CXsrcInstallerDlg::MakeDirectory(LPSTR path)
{

//parse directory
       CString directory = path;
	   CString dirArray[20];
	   int dirCount = 0;
       int fPathLength = directory.GetLength();
       
	   
	   for(int dLoop=(fPathLength-1);dLoop>0;dLoop--)
       {
           char xTest = directory.GetAt(dLoop);
	       if(xTest == '\\')
		   {
	         dirArray[dirCount++] = directory;
			   
			   //break;
			 directory.Delete(dLoop,1);
		   }
	       else
             directory.Delete(dLoop,1);
       }
       for(int i=(dirCount-1);i>-1;i--)
	   {
           CreateDirectory(dirArray[i],NULL);
	   }
}

void CXsrcInstallerDlg::OnBnClickedButtonUninstall()
{  
	 HKEY hKey;
	 LONG lRet;
     lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,"SOFTWARE\\microsoft\\windows\\currentversion\\uninstall\\"+m_appTitle,0, KEY_QUERY_VALUE, &hKey );
	 if( lRet != ERROR_SUCCESS )
	 {
         AfxMessageBox("Uninstaller has encountered problems, cannot find install registry entry");
		 OnOK();
         return;
	 }
     char installDir[500];
	 ZeroMemory(installDir,500);
	 DWORD dwBufLen=500;
	 lRet = RegQueryValueEx( hKey, "InstallPath", NULL, NULL,(LPBYTE) installDir, &dwBufLen);
     SetCurrentDirectory(installDir);
	 RegCloseKey( hKey );

	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open("uninstall.idt",CFile::modeRead, &exc);
	 if(res == FALSE){
	    AfxMessageBox("Uninstaller has encountered problems, cannot find install unistall data");
		OnOK();
		return;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);

     CString tempDir;
	 while(the_in_Archive.ReadString(tempDir))
	 {
          DeleteFile(tempDir);
	 }
	 the_in_Archive.Close();
	 fl.Close();
	 AfxMessageBox("Uninstall completed");
	 OnOK();
	 RegDeleteKey(HKEY_LOCAL_MACHINE,"SOFTWARE\\microsoft\\windows\\currentversion\\uninstall\\"+m_appTitle);
}

void CXsrcInstallerDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
void CXsrcInstallerDlg::OnDestroy()
{
	CDialog::OnDestroy();
    EnterCriticalSection( &m_cSection);
	m_closeNow = TRUE;
	LeaveCriticalSection( &m_cSection);
	Sleep(2000);

	if(m_lgThreadHandle)
	{
       CloseHandle(m_lgThreadHandle);
	}


	DeleteCriticalSection(&m_cSection);
	CoUninitialize();
}


HRESULT CXsrcInstallerDlg::CreateLink(CString lpszPathObj, 
									  CString lpszPathLink,
									  CString workingDirectoy,
									  CString lpszDesc) 
{ 
    HRESULT hres; 
    IShellLink* psl; 
 
    // Get a pointer to the IShellLink interface. 
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
                            IID_IShellLink, (LPVOID*)&psl); 
    if (SUCCEEDED(hres)) 
    { 
        IPersistFile* ppf; 
 
        // Set the path to the shortcut target and add the description. 
        hres = psl->SetPath(lpszPathObj); 
        hres = psl->SetDescription(lpszDesc); 
		hres = psl->SetWorkingDirectory(workingDirectoy);
 
        // Query IShellLink for the IPersistFile interface for saving the 
        // shortcut in persistent storage. 
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf); 
 
        if (SUCCEEDED(hres)) 
        { 
            WCHAR wsz[MAX_PATH]; 
 
            // Ensure that the string is Unicode. 
            MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH); 
			
            // TODO: Check return value from MultiByteWideChar to ensure 
                     //success.
 
            // Save the link by calling IPersistFile::Save. 
            hres = ppf->Save(wsz, TRUE); 
            ppf->Release(); 
        } 
        psl->Release(); 
    } 
    return hres; 
}

CString CXsrcInstallerDlg::GetSystemDrive()
{
  

  TCHAR  infoBuf[INFO_BUFFER_SIZE];
  DWORD  bufCharCount = INFO_BUFFER_SIZE;
 
  if( !GetSystemDirectory( infoBuf, INFO_BUFFER_SIZE ) )
	  return "C:";
  
  CString bufferCnv = infoBuf;
  CString retVal = "";
  retVal += bufferCnv.GetAt(0);
  retVal += bufferCnv.GetAt(1);
  return retVal;
}

void CXsrcInstallerDlg::OnTimer(UINT nIDEvent)
{
    CDialog::OnTimer(nIDEvent);
	BOOL close = FALSE;
	 EnterCriticalSection( &m_cSection);
     UpdateData(TRUE);

	 m_progressInstall.SetPos(m_globalPercDone);
	 if(this->m_closeNow)
		 close = TRUE;

	 if((GetTickCount()-m_LastTimeStampBytes) > 1000)
	 {
		 m_kbSec = (m_bytesDownloaded - m_lastbytesDownloaded);
	     m_lastbytesDownloaded = m_bytesDownloaded;
	     m_LastTimeStampBytes = GetTickCount();
	 }

	 m_statusOutput = m_statusPub;

	 if(m_kbSec > 0)
	 {
        m_kbout = "KB Sec:"+ConvToString(m_kbSec);
	 }
	 else
	    m_kbout = "";

	 UpdateData(FALSE);
	 LeaveCriticalSection( &m_cSection);
	 if(close)
	 {
	    close = FALSE;
		this->m_closeNow = FALSE;
	    CInstallComplete dlg;
	    dlg.DoModal();
	    OnOK();
	 }
}



//-----------------------------------------------------------//
DWORD WINAPI InstallIt(LPVOID lpParameter)
{	   
	CXsrcInstallerDlg * classObj = (CXsrcInstallerDlg *)lpParameter; 
	if(FAILED(CoInitialize(NULL)))
	{
        return 0;
	}
    //CCabinetObj cabObj;
	
    /*CResourceCabinet testCab;
	testCab.Init();
	testCab.CreateFDIContext();
	testCab.SetProgressCountRef(&classObj->m_globalPercDone);
	testCab.SetCriticalSection(&classObj->m_cSection);
	testCab.Copy("C:\\Projects\\XsrcInstaller\\InstallTest\\TestExtract\\setup.cab","C:\\Projects\\XsrcInstaller\\InstallTest\\TestExtract\\",NULL);
    return 0;*/


		//validate install path closing
    int validatelength = classObj->m_installPath.GetLength();
	if(validatelength < 1)
	{
       AfxMessageBox("Invalid path, please enter a valid path");
       return 0;
	}
	
    if(classObj->m_installPath.GetAt(validatelength-1) != '\\')
	   classObj->m_installPath += "\\";
    //end validate path closing
    char buffer[_MAX_PATH];
    _getcwd( buffer, _MAX_PATH );
    CString CurDir = buffer;
	int validatelength2 = CurDir.GetLength();
	if(CurDir.GetAt(validatelength2-1) != '\\')
	   CurDir += "\\";
	classObj->MakeDirectory(classObj->m_installPath.GetBuffer());
	SetCurrentDirectory(classObj->m_installPath);
	//create uninstall dat file handle
	CFileException execUninstallDat;
    CFile uninstallDatFileHandle;
	CArchive the_out_Archive(&uninstallDatFileHandle,CArchive::store);
    BOOL uninstallDatRes = uninstallDatFileHandle.Open(classObj->m_installPath + "uninstall.idt",CFile::modeCreate | CFile::modeWrite, &execUninstallDat);
   
    if(!uninstallDatRes)
	{//invalid path or dont have rights
		AfxMessageBox("Invalid path error#1002 , please enter a valid path");
        return 0;
	}//end invalid path or dont have rights
	

    //Download recent cabinet file
	while(1)
	{
	
    CHttpFileDownloader * fileStreamer = new CHttpFileDownloader();
	CString internetPath = "http://www.endlessages.com/FullClient/Setup.cab";
	//CString internetPath = "http://www.endlessages.com/trialclient/Setup.cab";
	DWORD downloadSize = fileStreamer->GetUrlByteSize("DLSession",internetPath.GetBuffer());
	EnterCriticalSection( &classObj->m_cSection);
	if(downloadSize > 1000000)
		classObj->m_statusPub = "Downloading Recent Version- File Size:"+classObj->ConvToString((downloadSize/1000000)) + "MB";
	LeaveCriticalSection( &classObj->m_cSection);
	CString fileTragetName = "Setup.cab";
	CString saveLocation = classObj->m_installPath + fileTragetName;
		if(fileStreamer->DownloadFileINET("patch Session",
											saveLocation.GetBuffer(),
											internetPath.GetBuffer(),
											&classObj->m_globalPercDone,
											NULL,
											&classObj->m_bytesDownloaded,
											&classObj->m_closeNow,
											&classObj->m_cSection)==TRUE)
		{
			if(fileStreamer->m_totalBytesDownloaded == fileStreamer->m_totalProgLength)
			{
				EnterCriticalSection( &classObj->m_cSection);
				classObj->m_statusPub = "Installing:"+classObj->ConvToString((fileStreamer->m_totalProgLength/1000000)) + "MB";
				classObj->m_bytesDownloaded = 0;
				LeaveCriticalSection( &classObj->m_cSection);
				//cabController.ExtractCabinetToDirectory(CurDir,tempSaveLocation,&classObj->m_percentagePub,&classObj->m_updateCriticalSection);
				/*cabObj.ExtractCabinetToDirectory(   classObj->m_installPath,
													classObj->m_installPath+fileTragetName,
													&classObj->m_globalPercDone,
													&classObj->m_cSection,
													the_out_Archive);*/

				CResourceCabinet testCab;
				testCab.Init();
				testCab.CreateFDIContext();
				testCab.SetProgressCountRef(&classObj->m_globalPercDone);
				testCab.SetCriticalSection(&classObj->m_cSection);
				CString combo = classObj->m_installPath+fileTragetName;
				testCab.Copy(combo.GetBuffer(),classObj->m_installPath,NULL);

				Sleep(3000);
				//delete temp file
				//AfxMessageBox("pause");
				DeleteFile(saveLocation);
				break;
			}
			else
			{
				//if(!classObj->m_closeNow)
				//   AfxMessageBox("Download failed, trying again...");

			}
		}
	delete fileStreamer;
	fileStreamer = 0;	
	}


	/*
	CCabinetObj cabObj;
	UINT installerSize = 573440;
	CFileException exc;
	CFile fl;
	BOOL res = fl.Open(CurDir+"Setup.exe",CFile::modeRead, &exc);
	if(!res)
	{
        CurDir = classObj->GetDeskTopFolder();
		CurDir += "Setup.exe";
		res = fl.Open(CurDir,CFile::modeRead, &exc);
        if(!res)
		{
		   AfxMessageBox("Cannot find file:"+CurDir+" *Try running application directly..");
           return 0;
		}
	} 




	UINT length = fl.GetLength();
	char * thisFileData = new char[length];
	fl.Read(thisFileData,length);
	fl.Close();
	
	
	//Extract data
	int tracer = installerSize;
	while(tracer < length)
	{
	    //m_progressInstall.SetPos(tracer);
	    
		SPackageHeader headerInfo;
        //parse headers
        CopyMemory(&headerInfo,&thisFileData[tracer],sizeof(SPackageHeader));
		tracer += sizeof(SPackageHeader);
        char * fileNameArray = new char[headerInfo.vDirNameLength+1];
		ZeroMemory(fileNameArray,headerInfo.vDirNameLength+1);
        CopyMemory(fileNameArray,&thisFileData[tracer],headerInfo.vDirNameLength);
		tracer += headerInfo.vDirNameLength;

	
	    //m_progressInstall.SetPos(tracer);
	
        //write file
		CString CurDir;
        char buffer[_MAX_PATH];
        _getcwd( buffer, _MAX_PATH );
		CurDir = buffer;
		//CString outputPath = "";
        CString fileName = fileNameArray;
		
		fileName.MakeLower();
		//CString dirName = fileName;
		//dirName.Replace(".cab","");
		//outputPath = m_installPath + dirName + "\\";
	
		//classObj->MakeDirectory(classObj->m_installPath.GetBuffer());
        CFileException exc2;
        CFile fl2;
        char * writeData = new char[headerInfo.dataBlockLength];
        CopyMemory(writeData,&thisFileData[tracer],headerInfo.dataBlockLength);
        tracer += headerInfo.dataBlockLength;

        BOOL res2 = fl2.Open(classObj->m_installPath+fileName,CFile::modeCreate | CFile::modeWrite, &exc);
	    fl2.Write(writeData,headerInfo.dataBlockLength);
	    fl2.Close();



        cabObj.ExtractCabinetToDirectory(classObj->m_installPath,
			                             classObj->m_installPath+fileName,
										&classObj->m_globalPercDone,
										&classObj->m_cSection,
										the_out_Archive);
        DeleteFile(classObj->m_installPath+"\\"+fileName);

		delete [] writeData;
		writeData = 0;
		delete [] fileNameArray;
		fileNameArray = 0;
		
	}

	delete [] thisFileData;
	thisFileData = 0;*/

	
	//set registry key
	HKEY hk = NULL; 
	if(RegCreateKey(HKEY_LOCAL_MACHINE,"SOFTWARE\\microsoft\\windows\\currentversion\\uninstall\\"+classObj->m_appTitle,&hk))
    {

	}
	if(!hk)
		return 0;
    if (RegSetValueEx(hk,             // subkey handle 
                        "DisplayName",       // value name 
                        0,                        // must be zero 
                        REG_EXPAND_SZ,            // value type 
                        (LPBYTE)classObj->m_appTitle.GetBuffer(),           // pointer to value data 
                        19))       // length of value data 
    {

    }
	int length = classObj->m_installPath.GetLength();
    if (RegSetValueEx(hk,             // subkey handle 
                        "InstallPath",       // value name 
                        0,                        // must be zero 
                        REG_EXPAND_SZ,            // value type 
                        (LPBYTE)classObj->m_installPath.GetBuffer(),           // pointer to value data 
                        length))       // length of value data 
    {
    }
	CString cnv = classObj->m_installPath + "Uninstaller.exe";
    length = cnv.GetLength();
	if (RegSetValueEx(hk,             // subkey handle 
                        "UninstallString",       // value name 
                        0,                        // must be zero 
                        REG_EXPAND_SZ,            // value type 
                        (LPBYTE)cnv.GetBuffer(),           // pointer to value data 
                        length))       // length of value data 
    {
    }
	//create shortcuts
    CString sysInstallDriveLetter = classObj->GetSystemDrive();
   

	HRESULT hres = classObj->CreateLink(classObj->m_installPath+"endlessages.exe", 
		                      sysInstallDriveLetter+"\\Documents and Settings\\All Users\\Desktop\\Endless Ages.lnk", 
							  classObj->m_installPath,
							  "Endless Ages");
	hres = classObj->CreateLink(classObj->m_installPath+"endlessages.exe", 
		                      sysInstallDriveLetter+"\\Documents and Settings\\All Users\\Start Menu\\Programs\\Endless Ages.lnk", 
							  classObj->m_installPath,
							  "Endless Ages");

    the_out_Archive.WriteString(sysInstallDriveLetter+"\\Documents and Settings\\All Users\\Desktop\\Endless Ages.lnk \n");
    the_out_Archive.WriteString(sysInstallDriveLetter+"\\Documents and Settings\\All Users\\Start Menu\\Programs\\Endless Ages.lnk \n");
	the_out_Archive.Close();
	uninstallDatFileHandle.Close();

    EnterCriticalSection( &classObj->m_cSection);
    classObj->m_statusPub = "Install Complete.";
	LeaveCriticalSection( &classObj->m_cSection);

	
	
	classObj->m_closeNow = TRUE;	
return 1;
}

CString CXsrcInstallerDlg::GetDeskTopFolder()
{
    IShellFolder * sFolder = NULL;
	HRESULT err = SHGetDesktopFolder(&sFolder);
    STRRET name;
	ITEMIDLIST listID;
    err = sFolder->GetDisplayNameOf(NULL,
		                            SHGDN_FORPARSING,//DWORD uFlags,
                                    &name);//LPSTRRET lpName


	/*char charArray[MAX_PATH];
	ZeroMemory(charArray,MAX_PATH);
	int length = GetLPWSTRLength(name.pOleStr);
	CopyMemory(charArray,name.pOleStr,length);*/
	CString folderName = ConvertLPWSTR(name.pOleStr);
	folderName += "\\";
    //delete sFolder;
	//sFolder = 0;
    return folderName;
}

CString CXsrcInstallerDlg::ConvertLPWSTR(LPWSTR & string)
{
	CString temp;
    int trace = 0;
	while(1)
	{
	   if(string[trace] == 0)
          break;

	   temp += string[trace];
	   //trace+=2;
	   trace++;

	}
    return temp;
}

CString CXsrcInstallerDlg::ConvToString(int number)
{    
              if(number==0)
				 return "0";
	 
	          int decimal,sign;
              char *buffer;
              CString temp;	 
	          buffer = _fcvt(number,0,&decimal,&sign);
    	      temp = buffer;
			  if(sign != 0)
                 temp = operator+("-",temp);
		
			  return temp;
}
void CXsrcInstallerDlg::OnEnChangeEditinstallationpath()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
