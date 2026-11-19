// CAppPatcherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CAppPatcher.h"
#include "CAppPatcherDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ENDLESS


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
public:
	afx_msg void OnDestroy();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CCAppPatcherDlg dialog

DWORD WINAPI Downloader(LPVOID lpParameter);


CCAppPatcherDlg::CCAppPatcherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCAppPatcherDlg::IDD, pParent)
	, m_currentDirectory(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCAppPatcherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_currentDirectory, m_currentDirectory);
	DDX_Control(pDX, IDC_EDIT_currentDirectory, m_currentDirEdit);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_nextButtonControl);
	DDX_Control(pDX, IDC_PictureButton, m_imageBtn);
	DDX_Control(pDX, IDC_PROGRESS_METER, m_progressMeter);
	DDX_Control(pDX, IDC_EDIT_username, m_username);
	DDX_Control(pDX, IDC_EDIT_Password, m_password);
	DDX_Control(pDX, IDC_COMBO_resolutions, m_resolutions);
	DDX_Control(pDX, IDC_CHECK_Fullscreen, m_fullscreen);
	DDX_Control(pDX, IDC_CHECK_SaveLogonData, m_saveLogonData);
	DDX_Control(pDX, IDC_COMBO_bitDepth, m_bitDepth);
	DDX_Control(pDX, IDC_COMBO_ServerSelection, m_serversList);
	DDX_Control(pDX, IDC_EXPLORER1, m_explorerCtrl);
}

BEGIN_MESSAGE_MAP(CCAppPatcherDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CCAppPatcherDlg::OnBnClickedOk)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CCAppPatcherDlg::OnBnClickedButtonNext)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CCAppPatcherDlg message handlers

BOOL CCAppPatcherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(FAILED(CoInitialize(NULL)))
	{
        return 0;
	}

	m_explorerCtrl.ShowWindow(FALSE);

	m_clientCAB				= "AXengineApp.cab";
	m_clientEXE				= "AXengineApp.exe";
	m_serverCount = 0;
    m_clientDownloadAttempts = 0;
	m_timeWaitCount = 0;
    char buffer[_MAX_PATH];
	_getcwd( buffer, _MAX_PATH );
	m_loadPointDirectory = buffer;
	m_loadPointDirectory += "\\";
	m_xEncrypt = new CAXEncryptionLib("qwertyuiopoiuytrewqwertyuiopoiuy");
    //MUST HARDCODE PER GAME
	UpdateData(TRUE);
    //default
	m_contentServerIP = "74.86.231.14";
	m_contentServerUrl = "http://74.86.231.14/contentmanager.aspx";
	m_decoratedGameName = "The Chronicle";
    m_gameName = "CHRONICLE";
	m_currentDirectory = "C:\\RapidRealityGames\\";
    //end default
	#ifdef ENDLESS
	m_clientCAB				= "EndlessAges.cab";
	m_clientEXE				= "EndlessAges.exe";
	m_contentServerIP		= "67.228.19.248"; //"74.86.231.14";
	m_contentServerUrl		= "http://67.228.19.248/contentmanager.aspx";
	m_decoratedGameName		= "EndlessAgesR";
    m_gameName				= "ENDLESS";
	m_currentDirectory		= "C:\\EndlessAgesR\\";
    #endif
	/*#ifdef CHRONICLE
	m_contentServerIP = "72.29.80.117";
	m_contentServerUrl = "http://72.29.80.117/contentmanager.aspx";
	m_decoratedGameName = "The Chronicle";
    m_gameName = "CHRONICLE";
	m_currentDirectory = "C:\\GameWorlds\\";
    #endif */
	
	UpdateData(FALSE);
	//END MUST HARDCODE PER GAME
	m_uninstallData.m_logFileName = m_gameName+".idt";
	m_axLogger.m_logFileName = m_gameName + ".log";
	

	InitializeCriticalSection(&m_section);
	
	m_currentDirEdit.ShowWindow(0);
	m_progressMeter.ShowWindow(0);
	
	 ResolutionPanelVisibility(0);
	m_progressMeter.SetRange32(0,100);
    //fill in available resoultions-
	//todo:: we should run some monitor caps here
	m_resolutions.AddString("1024X768");
	m_resolutions.AddString("1280X800");
	m_resolutions.AddString("1280X1024");
	m_resolutions.AddString("1152X864");
	m_resolutions.AddString("1600X900");
	m_resolutions.AddString("1600X1200");

	m_bitDepth.AddString("16");
	m_bitDepth.AddString("32");

	m_bitDepth.SetCurSel(1);
	m_resolutions.SetCurSel(1);
	m_mode = 0;
	m_currentProgress = 0;
	//m_currentResource = IDR_JPEG_TreeFolk;
	ChangeBackground(IDR_JPEG_Splash);
	//load all needed images
	//HBITMAP bitMapTemp = m_imageAx.LoadAnImage("",IDR_JPEG_TreeFolk);
    //m_installerBackgroundImage.Attach(bitMapTemp);
	// Add "About..." menu item to system menu.

	m_devMode = 0;
	m_modeEngaged = -1;
	CString devMode = m_abTool.GetSoftwareRegkeyVal("ENDLESS_AGESR","DEV_ON");
	if(devMode.GetLength() > 0)
       m_devMode = 1;

    DWORD curVersion = 0x00090003;
	m_mustInstallDirectX = FALSE;
    if(!DetectDirectXVersionReq(curVersion,""))
	    m_mustInstallDirectX = TRUE;

	//check if installed already
	m_installPath = m_abTool.GetSoftwareRegkeyVal("ENDLESS_AGESR","INSTALL_PATH");
    if(!SetCurrentDirectory(m_installPath))
	{//install required
	   m_mode = 1;
	}//end install required
	else
	{//verify client then show logon UI
       m_mode = 2;
	   m_axLogger.WriteDebugFile(("starting up:"+m_gameName).GetBuffer(),1,1);
	}//end verify client then show logon UI



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
	SetIcon(m_hIcon, TRUE);		// Set small icon


	// TODO: Add extra initialization here
	SetTimer(1,500,0);

	
	
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}
//-----------------------------------------------------------//
void CCAppPatcherDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


void CCAppPatcherDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//UpdateBackImage(IDR_JPEG_TreeFolk);

	     CBitmap localMap,*pOldBmp;
	    HBITMAP bitMapTemp = m_imageAx.LoadAnImage("",m_currentResource);
        localMap.Attach(bitMapTemp);

		CDC BmpDc;
		VERIFY( BmpDc.CreateCompatibleDC(&dc) );
		RECT rect;
		::GetClientRect(GetSafeHwnd(),&rect);
		pOldBmp = (CBitmap *)BmpDc.SelectObject(&localMap);
		dc.StretchBlt(0,0,rect.right,rect.bottom,&BmpDc,0,0,636,471,SRCCOPY);
		BmpDc.SelectObject(pOldBmp);

		
	    
		
	
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCAppPatcherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//-----------------------------------------------------------//
void CCAppPatcherDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
//-----------------------------------------------------------//
void CAboutDlg::OnDestroy()
{
	 CDialog::OnDestroy();

	
}
//-----------------------------------------------------------//
void CCAppPatcherDlg::OnDestroy()
{
	CDialog::OnDestroy();
	m_installerBackgroundImage.DeleteObject();

	CoUninitialize();
	
}

//------------------Direct X Version detection---------------------//
BOOL CCAppPatcherDlg::DetectDirectXVersionReq(DWORD minVersion,CString errorMsg)
{
    BOOL requirementMet			= FALSE;
	HRESULT						hr;
    //TCHAR						trResult[128];
    DWORD						dwDirectXVersion = 0;
    TCHAR						strDirectXVersion[10];
	GETDXverObj					verObj;
    hr = verObj.GetDXVersionPub( &dwDirectXVersion, strDirectXVersion, 10 );
    if( SUCCEEDED(hr) )
    {
        //if( dwDirectXVersion >= 0x00090003)
        //    requirementMet = TRUE;
		if( dwDirectXVersion >= minVersion)
            return TRUE;
    }

    if(!requirementMet)
	{
		if(errorMsg.GetLength() > 0)
           AfxMessageBox(errorMsg);
	}
	return FALSE;
}
//-----------------------------------------------------------//
void CCAppPatcherDlg::OnTimer(UINT_PTR nIDEvent)
{
	//m_modeEngaged = -1;
	//m_mode = 1;
	CString temp;
	char * buffer = NULL;
	DWORD returnLength = 0;
	
	if(m_timeWaitCount < 4)
	{//wait for splash to show
       m_timeWaitCount++;
	   return;
	}//end wait for splash to show
       

	switch(m_mode)
	{
	case 1://installation install directory
       if(m_mode == m_modeEngaged)
	      break;
	   else
	   {
		   m_modeEngaged = m_mode;
	       ChangeBackground(IDR_JPEG_InstallPath);
		   m_currentDirEdit.ShowWindow(1);
		   m_buttonList.AddButton(IDR_NEXTUP,IDR_NEXTOVER,IDR_NEXTDOWN,m_imageBtn,IDC_PictureButton,1);
	   }
	break;
    case 2://verify client then show logon UI
	   if(m_mode == m_modeEngaged)
		   break;
	   else
	   {//process this
		    m_modeEngaged = m_mode;
	       
		   m_progressMeter.SetPos(0);
           //see if patcher needs to be downloaded
           if(m_loadPointDirectory.CompareNoCase(m_installPath)!=0)
		   {
			   ChangeBackground(IDR_JPEG_Installing);
               //install shoprtcuts
               //start uninstall data
               m_uninstallData.WriteDebugFile((m_installPath+m_uninstallData.m_logFileName).GetBuffer(),1,1);
			   m_uninstallData.WriteDebugFile((m_installPath+m_gameName+".exe").GetBuffer(),0,1);

               CreateShortcuts(m_installPath+m_gameName+".exe", 
							   m_decoratedGameName,
							   m_installPath);
			   CreateUnistallLinks(m_decoratedGameName, m_installPath,m_gameName);

			   m_loadPointDirectory = m_installPath;
               temp = m_contentServerUrl + "?installer_patcher="+m_gameName;   
			   buffer = NULL;
			   CHttpFileDownloader urlReader;
			   urlReader.m_asciMode = TRUE;
			   if(urlReader.GetUrlData("iexplorer data",temp.GetBuffer(),(BYTE**)&buffer,&returnLength))
			   {//succeeded
			       
			      ULONGLONG pos = 0;
				  CString urlTemp = ParseLine(pos,buffer,returnLength,0);
				  CString cabNameTemp = ParseLine(pos,buffer,returnLength,0);
				  m_axLogger.WriteDebugFile(("Download Set:"+cabNameTemp).GetBuffer(),0,0);
				  DownloadCabinetAndExtract(urlTemp,cabNameTemp);
				  if(returnLength > 0)
					 delete [] buffer;
					 buffer = 0;
			   }
			   else
			      m_axLogger.WriteDebugFile(("GetUrlData() FAILED:"+temp).GetBuffer(),0,0);

              break;
		   }
           //see if client needs to be updated
		   //get CRC
		   buffer = NULL;
		   CHttpFileDownloader urlReaderCRC;
		   urlReaderCRC.m_asciMode = TRUE;
		   temp = m_contentServerUrl + "?contenttype=axengineapp";
		   if(urlReaderCRC.GetUrlData("iexplorer data",temp.GetBuffer(),(BYTE**)&buffer,&returnLength))
		   {//aquired CRC string
			   CString	fileNameTemp = ""; 
			   long		fileSizeTemp = 0;
			   CString	cabNameTemp = "";
			   CString	offsetDirTemp = ""; 
			   unsigned long CRCTemp = 0;
			   unsigned long CAB_CRCTemp = 0;
			   int      currentSet = 0;
			   BOOL     fileSizeVerificationRequired = TRUE;
			   BOOL     abort = FALSE;
			   CString stringToParse = buffer;
			   if(returnLength > 0)
			   {
				  delete [] buffer;
				  buffer = 0;
			   }

			   if(ParseUrlStringToData(0, 
									   stringToParse,//CString urlParse, 
									   fileNameTemp, 
									   &fileSizeTemp, 
									   cabNameTemp, 
									   offsetDirTemp, 
									   &CRCTemp,
									   &CAB_CRCTemp,
									   &fileSizeVerificationRequired))
			   {//parsed CRC info correctly
				   if(ValidateFile(fileNameTemp, CRCTemp, TRUE) == false && m_clientDownloadAttempts < 2)//redownload once
				   {//check CRC
					   ChangeBackground(IDR_JPEG_installingClient);
					   temp = m_contentServerUrl + "?gameapp=1";
					   DeleteFile(fileNameTemp);
					   Sleep(2000);
					   buffer = NULL;
					   CHttpFileDownloader urlReader;
					   urlReader.m_asciMode = TRUE;
					   if(urlReader.GetUrlData("iexplorer data",temp.GetBuffer(),(BYTE**)&buffer,&returnLength))
					   {//succeeded
						  DownloadCabinetAndExtract(buffer,m_clientCAB);
						  if(returnLength > 0)
							 delete [] buffer;
							 buffer = 0;
					   }
					   //Sleep(2000);
					   m_clientDownloadAttempts++;
					   break;
				   }//end check CRC
			   }//end parsed CRC info correctly
		   }//end aquired CRC string
		   
           //initialize server list

		   //Last open logon UI
           ResolutionPanelVisibility(1);
		   //m_buttonList.AddButton(IDR_JPEG_ConnectUp,IDR_JPEG_ConnectHover,IDR_JPEG_ConnectDown,m_imageBtn,IDC_PictureButton,2);
		   m_buttonList.AddButton(IDR_NEXTUP,IDR_NEXTOVER,IDR_NEXTDOWN,m_imageBtn,IDC_PictureButton,2);

		   m_explorerCtrl.ShowWindow(TRUE);
		   m_explorerCtrl.Navigate("https://www.endless-ages.com/trading-post/login.aspx",NULL,NULL,NULL,NULL);
           

	   }//end process this
    break;
	case 3://downloading file
	   m_modeEngaged = m_mode;
	   EnterCriticalSection(&m_section);
	   m_progressMeter.SetPos(m_currentProgress);

       LeaveCriticalSection(&m_section);
	   if(m_currentProgress == 100)
	   {
	      m_mode = 2;//begin reverification
		  m_progressMeter.ShowWindow(FALSE);
	   }
	break;
	}

	CDialog::OnTimer(nIDEvent);
}
//-----------------------------------------------------------//
void CCAppPatcherDlg::OnBnClickedButtonNext()
{
	m_mode++;
}
//-----------------------------------------------------------//
void CCAppPatcherDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	BOOL msDwn = FALSE;
	if(MK_LBUTTON & nFlags)
	   msDwn = TRUE;
	
	int function = m_buttonList.Evaluate(point, msDwn, this);
    FunctionProcessor(function);
	CDialog::OnMouseMove(nFlags, point);

	//RECT rectTemp;
	//GetWindowRect(&rectTemp);

	
}
//-----------------------------------------------------------//
void CCAppPatcherDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL msDwn = FALSE;
	if(MK_LBUTTON & nFlags)
	   msDwn = TRUE;
	
	int function = m_buttonList.Evaluate(point, 1, this);
	CDialog::OnLButtonDown(nFlags, point);
	
}
//-----------------------------------------------------------//
void CCAppPatcherDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	BOOL msDwn = FALSE;
	if(MK_LBUTTON & nFlags)
	   msDwn = TRUE;
	
	int function = m_buttonList.Evaluate(point, 0, this);
    FunctionProcessor(function);
	CDialog::OnLButtonUp(nFlags, point);
	
}
//-----------------------------------------------------------//
BOOL CCAppPatcherDlg::WriteCoinitFile(CString username,
									  CString password,
									  CString game,
									  CString downloadUrl,
									  CString gameserverIP,
									  BOOL fullscreen,
									  BOOL saveLogonData,
									  CString wResolution,
									  CString hResolution,
									  CString depth)
{
	       CFileException exc;
           CFile fl;
           BOOL res = fl.Open("coinit.tmp",CFile::modeCreate | CFile::modeWrite, &exc);
		   if(!res)
		   {
			   Sleep(2000);
               res = fl.Open("coinit.tmp",CFile::modeCreate | CFile::modeWrite, &exc);
		   } 
		   if(!res)
		   {//failed to create temp file
			   m_axLogger.WriteDebugFile("ERROR 21::could not create coinit.tmp file",0,1);
              return FALSE;
		   }//end failed to create temp file
		   else
		   {
               //username
			   CString tempStr,newLine;
			   newLine = "\r\n";
			   CString tempWrite = m_xEncrypt->EncryptString64(username.GetBuffer());
			   fl.Write(tempWrite.GetBuffer(),tempWrite.GetLength());
			   fl.Write(newLine.GetBuffer(),newLine.GetLength());
			   //password
			   tempWrite = m_xEncrypt->EncryptString64(password.GetBuffer());
			   fl.Write(tempWrite.GetBuffer(),tempWrite.GetLength());
			   fl.Write(newLine.GetBuffer(),newLine.GetLength());
			   //game
			   tempWrite = m_xEncrypt->EncryptString64(game.GetBuffer());
			   fl.Write(tempWrite.GetBuffer(),tempWrite.GetLength());
			   fl.Write(newLine.GetBuffer(),newLine.GetLength());
			   //download URL
			   tempWrite = m_xEncrypt->EncryptString64(downloadUrl.GetBuffer());
			   fl.Write(tempWrite.GetBuffer(),tempWrite.GetLength());
			   fl.Write(newLine.GetBuffer(),newLine.GetLength());
			   //Game Server IP
               tempWrite = m_xEncrypt->EncryptString64(gameserverIP.GetBuffer());
			   fl.Write(tempWrite.GetBuffer(),tempWrite.GetLength());
			   fl.Write(newLine.GetBuffer(),newLine.GetLength());
			   //fullscreen
			   tempWrite = "0";
			   if(fullscreen==1)
				  tempWrite = "1";
			   fl.Write(tempWrite.GetBuffer(),tempWrite.GetLength());
			   fl.Write(newLine.GetBuffer(),newLine.GetLength());
			   //save user logon data
			   tempWrite = "0";
			   if(saveLogonData==1)
				  tempWrite = "1";
			   fl.Write(tempWrite.GetBuffer(),tempWrite.GetLength());
			   fl.Write(newLine.GetBuffer(),newLine.GetLength());
			   //resolution parse
               //w resolution
			   fl.Write(wResolution.GetBuffer(),wResolution.GetLength());
			   fl.Write(newLine.GetBuffer(),newLine.GetLength());
			   //H resolution
               fl.Write(hResolution.GetBuffer(),hResolution.GetLength());
			   fl.Write(newLine.GetBuffer(),newLine.GetLength());
			   //Bit Depth
			   fl.Write(depth.GetBuffer(),depth.GetLength());
			   fl.Write(newLine.GetBuffer(),newLine.GetLength());
			   
			   fl.Close();
			   return TRUE;
		   }
	   return FALSE;
}
//-----------------------------------------------------------//
void CCAppPatcherDlg::FunctionProcessor(int function)
{
    switch(function)
	{
	   case 1://set up install directories and download newest patcher and stand alone client
		   UpdateData(TRUE);
	        CreateInstallRegistries(m_currentDirectory);
           UpdateData(FALSE);
		   m_currentDirEdit.ShowWindow(0);
		   m_buttonList.RemoveButton(IDC_PictureButton);
		   //swicth mode to downloading patch and stand alone client
		   m_mode = 2;//verify client then show connect info
	   break;
       case 2://write connect info encrypted-start application-close
	       
               CString username,password,gameServerIP,gamename;
			   m_username.GetWindowTextA(username);
			   m_password.GetWindowTextA(password);
			   //Game Server IP
			   char * buffer = NULL;
			   DWORD returnLen = 0;
			   UpdateData(TRUE);
               int serverSel = m_serversList.GetCurSel();
			   UpdateData(FALSE);
               if(serverSel > -1 && serverSel < m_serverCount)
			   {
					gameServerIP		= m_serverListBuf[serverSel].serverIP;
					gamename			= m_serverListBuf[serverSel].gameName;

					m_axLogger.WriteDebugFile("Dynamic Game Name:",0,0);
					m_axLogger.WriteDebugFile(gamename.GetBuffer(),0,1);
			   }

			   /*CHttpFileDownloader urlReaderCRC;
			   urlReaderCRC.m_asciMode = TRUE;
			   CString tempUrlReq = m_contentServerUrl + "?gameserver=1&game="+m_gameName;
			   if(urlReaderCRC.GetUrlData("iexplorer data",tempUrlReq.GetBuffer(),(BYTE**)&buffer,&returnLen))
			   {
                  gameServerIP = buffer;//game server routing here
                  delete [] buffer;
				  buffer = 0;
			   }
			   else
			      m_axLogger.WriteDebugFile("Could Not Aquire Game Server IP FunctionProcessor()",0,1);*/

			   //resolution parse
			   CString resString,width,hieght,depth;
			   m_resolutions.GetWindowTextA(resString);
			   int parsePos = 0;
			   int mode = 0;
			   while(parsePos < resString.GetLength())
			   {
                   if(resString.GetAt(parsePos) == 'X')
				   {
                      mode = 1;
					  parsePos++;
					  continue;
				   }
				   if(mode == 0)
				      width += resString.GetAt(parsePos);
				   else
                      hieght += resString.GetAt(parsePos);

				   parsePos++;
			   }
			   m_bitDepth.GetWindowTextA(depth);


			   if(!WriteCoinitFile( username,
									password,
									gamename,
									m_contentServerIP,
									gameServerIP,
									m_fullscreen.GetCheck(),
									m_saveLogonData.GetCheck(),
									width,
									hieght,
									depth))
			   {
                  AfxMessageBox("ERROR 55 Cannot write config tmp file",0,0);
                  break;
			   }
			   
			   ExecApplication(m_clientEXE);//start game app
			   
			   OnOK();//close patcher
	   break;

	}
}
//-----------------------------------------------------------------//
BOOL CCAppPatcherDlg::GetInfoFromCoinitFile(CString & username,
									   CString & password,
									   CString & gameName,
									   CString & contentServerIP,
									   CString & gameServerIP,
									   BOOL & fullScreen,
									   BOOL & saveLogonData,
									   int & width,
									   int & height,
									   int & depth
									   )
{
	 CFileException exc;
     CFile fl;
	 BOOL res = fl.Open("coinit.tmp",CFile::modeRead, &exc);
	 if(res)
	 {
		ULONGLONG length = fl.GetLength();
        char * buf = new char[length+1];
		ZeroMemory(buf,length+1);
		fl.Read(buf,length);
		fl.Close();
		ULONGLONG pos = 0;

		username = m_xEncrypt->DecryptString64(ParseLine(pos,buf,length,1).GetBuffer());
		password = m_xEncrypt->DecryptString64(ParseLine(pos,buf,length,1).GetBuffer());
		gameName = m_xEncrypt->DecryptString64(ParseLine(pos,buf,length,1).GetBuffer());
		contentServerIP = m_xEncrypt->DecryptString64(ParseLine(pos,buf,length,1).GetBuffer());
		gameServerIP = m_xEncrypt->DecryptString64(ParseLine(pos,buf,length,1).GetBuffer());
        fullScreen = atoi(ParseLine(pos,buf,length,0));
		saveLogonData = atoi(ParseLine(pos,buf,length,0));
		CString widthStr = ParseLine(pos,buf,length,0);
		width = atoi(widthStr);
		CString heightStr = ParseLine(pos,buf,length,0);
		height = atoi(heightStr);
		CString depthStr = ParseLine(pos,buf,length,0);
		depth = atoi(depthStr);

		delete [] buf;
		buf = 0;

		if(saveLogonData == 0)
		{//clear file
		   password = "";
           if(!WriteCoinitFile(username,
							   "",
							   gameName,
							   contentServerIP,
							   gameServerIP,
							   fullScreen,
							   saveLogonData,
							   widthStr,
							   heightStr,
							   depthStr))
							   m_axLogger.WriteDebugFile("ERROR 76::Could Not Clear Coinit File",0,1);

		}//end clear file
		return TRUE;
	 }
	 return FALSE;
	 
}
//-----------------------------------------------------------//
CString CCAppPatcherDlg::ParseLine(ULONGLONG & pos,char * Buf, ULONGLONG length, int sqCount)
{
	 CString build;
	 int curLineC = 0;
     while(pos < length)
	 {
           build += Buf[pos];
		   pos++;

		   if(Buf[pos] == '\r')
		      if(Buf[pos+1] == '\n')
			  {
			     curLineC++;
				 if(curLineC > sqCount)
                    break;
			  }

	 }
	 pos++;
	 pos++;
     return build;
}
//-----------------------------------------------------------//
void CCAppPatcherDlg::CreateInstallRegistries(CString installPath)
{
        m_axLogger.WriteDebugFile("Set Content Path:",0,0);
	    m_axLogger.WriteDebugFile(installPath.GetBuffer(),0,1);
	    m_installPath = installPath;
	    CString tempDir = m_installPath+"CACHE\\";
	    m_abTool.MakeDirectory(tempDir.GetBuffer());
	    tempDir = m_installPath+"GameFiles\\";
	    m_abTool.MakeDirectory(tempDir.GetBuffer());
	    tempDir = m_installPath+"GameFiles\\";
	    m_abTool.MakeDirectory(tempDir.GetBuffer());
	    tempDir = m_installPath+"MapsModels\\";
	    m_abTool.MakeDirectory(tempDir.GetBuffer());
	    tempDir = m_installPath+"Sounds\\";
	    m_abTool.MakeDirectory(tempDir.GetBuffer());
	    m_abTool.SetSoftwareReg("ENDLESS_AGESR","INSTALL_PATH",m_installPath.GetBuffer());
		SetCurrentDirectory(m_installPath.GetBuffer());
}

//-----------------------------------------------------------//
DWORD WINAPI Downloader(LPVOID lpParameter)
{	    
      //NOTE
	  // classObj->m_cabNameCurrentDld ::MUST BE SET
	  // classObj->m_urlOfCurrentDownload ::MUST BE SET
	  CCAppPatcherDlg * classObj = (CCAppPatcherDlg *)lpParameter;   
	  CHttpFileDownloader * urlReader = new CHttpFileDownloader();
	  DWORD returnLength = 0;
	  char * buffer = NULL;
	  
	  BOOL result = urlReader->GetUrlDataLocked("iexplorer data",classObj->m_urlOfCurrentDownload.GetBuffer(),(BYTE**)&buffer,&returnLength,&classObj->m_currentProgress,&classObj->m_section);
	  if(result)
	  {
		  CString saveLocationWFile = classObj->m_installPath + classObj->m_cabNameCurrentDld;
		  //save to file 
          CFileException exc;
          CFile fl;
          BOOL res = fl.Open(saveLocationWFile,CFile::modeCreate | CFile::modeWrite, &exc);
          fl.Write(buffer,returnLength);
          fl.Close(); 
		  Sleep(100);
		  if(returnLength > 0)
		  {//delete mem block
             delete [] buffer;
			 buffer = 0;
		  }//end delete mem block
		  //extract cabinet to install dir
		  CResourceCabinet newCabObj;
		  newCabObj.Init();
		  newCabObj.CreateFDIContext();
		  int progLoc = 0;
		  newCabObj.SetProgressCountRef(&progLoc);
		  newCabObj.SetCriticalSection(&classObj->m_section);
		  
		  if(newCabObj.Copy(saveLocationWFile.GetBuffer(),classObj->m_installPath,NULL))
		  {//extract sucess 


		  }//end extract sucess
		  else
		  {//extract failed


		  }//end extract failed
		  //delete src cab
		  DeleteFile(saveLocationWFile.GetBuffer());
	  }
      Sleep(500);
	  delete urlReader;
	  urlReader = 0;

	  return 0;
}
//-----------------------------------------------------------//
void CCAppPatcherDlg::DownloadCabinetAndExtract(CString url,CString cabName)
{
	m_progressMeter.ShowWindow(TRUE);
    m_cabNameCurrentDld = cabName;
	m_urlOfCurrentDownload = url;
	m_mode = 3;
    m_dlHandle = CreateThread(NULL,0,Downloader,this,0,&m_dlThreadID);//call validate/dowloader
	
}

//-----------------------------------------------------------//
bool CCAppPatcherDlg::ParseUrlGetLine(int & curPos, CString & urlData, CString & lineString)
{
	 int length = urlData.GetLength();
	 lineString = "";
     while(curPos < length)
	 {
		 if(urlData.GetAt(curPos) == '\r')
		 {
		    curPos++;
			curPos++;
		    return true;
		 }
         lineString += urlData.GetAt(curPos);
		 curPos++;
	 } 
	 return false;
}
//-----------------------------------------------------------//
bool CCAppPatcherDlg::ParseUrlStringToData(int set, 
											  CString urlParse, 
											  CString & fileName, 
											  long * fileSize, 
											  CString & cabName, 
											  CString & offsetDir, 
											  unsigned long * CRC,
											  unsigned long * cab_CRC,
											  BOOL * fileSizeVerificationRequired)
{
    int lineSkip = 7*set;
	int currentPos = 0;
	int currentLine = 0;
	CString currentLineData = "";
	int lengthTotal = urlParse.GetLength();
	int varIndex = 0;
    /*while(ParseUrlGetLine(currentPos, urlParse, currentLineData))
	{
        if(currentLine == lineSkip)
		   break;

		currentLine++;
	}*/

    while(ParseUrlGetLine(currentPos, urlParse, currentLineData))
	{
        if(currentLine < lineSkip)
		{
		   currentLine++;
		   continue;
		}

		if(varIndex == 0)
           fileName	 = currentLineData;
		else if(varIndex == 1)
           *fileSize  = (long)atol(currentLineData.GetBuffer());
		else if(varIndex == 2)
           cabName	 = currentLineData;
		else if(varIndex == 3)
		   offsetDir = currentLineData;
		else if(varIndex == 4)
		{
		   *CRC		 = (unsigned long)_atoi64(currentLineData.GetBuffer());
		}
		else if(varIndex == 5)
		{
		   *fileSizeVerificationRequired		 = (BOOL)atoi(currentLineData.GetBuffer());//file CRC
		}
		else if(varIndex == 6)
		{
		   *cab_CRC	 = (unsigned long)_atoi64(currentLineData.GetBuffer());//cab crc
		   return true;
		}
		currentLine++;
		varIndex++;
	}
	return false;
}
//-----------------------------------------------------------------------------------------------------
bool CCAppPatcherDlg::ValidateFile(CString & fullPath, unsigned long fileCRC, BOOL verifyCRC)
{
	
	 /*if(m_crcGenerator.getcrc(fullPath)==fileCRC)
	    return true;

	 return false;*/

     CFileException exc;
     CFile fl;
	 BOOL res = fl.Open(fullPath,CFile::modeRead, &exc);
	 if(res == FALSE)
	 {
        if(exc.m_cause == CFileException::fileNotFound)
		{
		 
	       return false;
		}
		Sleep(50);
		res = fl.Open(fullPath,CFile::modeRead, &exc);
		 if(!res)
		   return false;
	 }
      //validate file size
     if(!verifyCRC)
        return true;

	 unsigned long cfileCRC = m_crcGenerator.getcrc(&fl);
	 if(cfileCRC==fileCRC)
	 {//all good
        fl.Close();
		return true;
	 }//end all good
	 fl.Close();
     return false;
}

//-----------------------------------------------------------------------------------------------------
const enum LOGIN_ACCESS_TYPES : unsigned char
{
	LOGIN_INTERNAL = 0,
	LOGIN_EXTERNAL,
	LOGIN_ALL
};

// NOTE: This value is set at compile time to determine what server access
//	the login client executable built will have.
#define LOGIN_ACCESS LOGIN_ALL

BOOL CCAppPatcherDlg::ResolutionPanelVisibility(BOOL visible)
{
	ChangeBackground(IDR_JPEG_CONNECT);
	m_serversList.ShowWindow(visible);
    if(visible)
	   if(m_serversList.GetCount()==0)
	   {
	      char * buffer = NULL;
	      DWORD returnLength = 0;
	      CString temp = m_contentServerUrl + "?server_x=1";//="+m_gameName;

		  switch (LOGIN_ACCESS)
		  {
			case LOGIN_ALL:
			{
				temp += "&access_type=all";
				break;
			}
			case LOGIN_INTERNAL:
			{
				temp += "&access_type=internal";
				break;
			}
			case LOGIN_EXTERNAL:
			default:
			{
				temp += "&access_type=external";
				break;
			}
		  }
		 
		  CHttpFileDownloader urlReader;
		  urlReader.m_asciMode = TRUE;
		  if(urlReader.GetUrlData("iexplorer data",temp.GetBuffer(),(BYTE**)&buffer,&returnLength))
		  {//succeeded
		      ULONGLONG pos = 0;
			  m_serverCount = 0;
			  while(pos < returnLength)
			  {
				  m_serverListBuf[m_serverCount].serverName = ParseLine(pos,buffer,returnLength,0);//get game server decorated name
				  m_serverListBuf[m_serverCount].serverIP = ParseLine(pos,buffer,returnLength,0);//get game server ip
				  m_serverListBuf[m_serverCount].gameName = ParseLine(pos,buffer,returnLength,0);//get game name
				  m_serversList.AddString(m_serverListBuf[m_serverCount].serverName);
				  m_serverCount++;
			  }
			  if(returnLength > 0)
				 delete [] buffer;
				 buffer = 0;

			  m_serversList.SetCurSel(0);
		  }
	      else
		     m_axLogger.WriteDebugFile(("GetUrlData() FAILED:"+temp).GetBuffer(),0,0);
	   }
	
	m_username.ShowWindow(visible);
	m_resolutions.ShowWindow(visible);
	m_fullscreen.ShowWindow(visible);
	m_password.ShowWindow(visible);
	m_saveLogonData.ShowWindow(visible);
	m_bitDepth.ShowWindow(visible);

	if(visible)
	{
	//possibly load defaults
	CString tempLoad,tempLoad2,tempLoad3,tempLoad4,tempLoad5,tempLoad6;
	BOOL localFullscreen = 1;
	BOOL saveLogonDataLoc = 0;
    int width, hieght, depth;
	GetInfoFromCoinitFile(tempLoad,
						  tempLoad2,
					      tempLoad3,
						  tempLoad4,
						  tempLoad5,
						  localFullscreen,
						  saveLogonDataLoc,
						  width,
						  hieght,
						  depth);
	m_username.SetWindowTextA(tempLoad);
	m_password.SetWindowTextA(tempLoad2);
	m_fullscreen.SetCheck(localFullscreen);
	m_saveLogonData.SetCheck(saveLogonDataLoc);

	

	CString wString,hString,dString;
    wString.Format("%i",width);
	hString.Format("%i",hieght);
	dString.Format("%i",depth);
	m_resolutions.SelectString(0,wString+"X"+hString);
	m_bitDepth.SelectString(0,dString);
	}
	
	return TRUE;
}
//-----------------------------------------------------------------------------------------------------
BOOL CCAppPatcherDlg::ExecApplication(CString appExe)
{
    char *args[4], prog[80];
    ZeroMemory(&prog,80);
 
    CopyMemory(&prog,appExe.GetBuffer(),appExe.GetLength());

	args[0] = prog;
    args[1] = "exec??";
    args[2] = "two";
    args[3] = NULL;

    _execv( prog,args );
	return TRUE;
}
//-----------------------------------------------------------------------------------------------------
BOOL CCAppPatcherDlg::CreateShortcuts(CString applicationWPath, 
									  CString shortcutName,
									  CString installPath)
{
    //create shortcuts
    CString sysInstallDriveLetter = GetSystemDrive();
   

	HRESULT hres = CreateLink(applicationWPath, 
		                                sysInstallDriveLetter+"\\Documents and Settings\\All Users\\Desktop\\"+shortcutName+".lnk", 
							            installPath,
							            shortcutName);
	hres = CreateLink(applicationWPath, 
		                        sysInstallDriveLetter+"\\Documents and Settings\\All Users\\Start Menu\\Programs\\"+shortcutName+".lnk", 
							    installPath,
							    shortcutName);

	return TRUE;
}
//-----------------------------------------------------------------------------------------------------
CString CCAppPatcherDlg::GetSystemDrive()
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
//-----------------------------------------------------------------------------------------------------
HRESULT CCAppPatcherDlg::CreateLink(CString lpszPathObj, 
									  CString lpszPathLink,//path to link
									  CString workingDirectoy,
									  CString lpszDesc) 
{ 
    HRESULT hres; 
    IShellLink* psl; 

	m_uninstallData.WriteDebugFile(lpszPathLink.GetBuffer(),0,1);
 
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
//-----------------------------------------------------------------------------------------------------
HRESULT CCAppPatcherDlg::CreateUnistallLinks(CString decoratedName, CString installPath, CString gameName)
{
HKEY hk = NULL; 
	if(RegCreateKey(HKEY_LOCAL_MACHINE,"SOFTWARE\\microsoft\\windows\\currentversion\\uninstall\\"+decoratedName,&hk))
    {

	}
	if(!hk)
		return 0;
    if (RegSetValueEx(hk,             // subkey handle 
                        "DisplayName",       // value name 
                        0,                        // must be zero 
                        REG_EXPAND_SZ,            // value type 
                        (LPBYTE)decoratedName.GetBuffer(),           // pointer to value data 
                        19))       // length of value data 
    {

    }
	int length = installPath.GetLength();
    if (RegSetValueEx(hk,             // subkey handle 
                        "InstallPath",       // value name 
                        0,                        // must be zero 
                        REG_EXPAND_SZ,            // value type 
                        (LPBYTE)installPath.GetBuffer(),           // pointer to value data 
                        length))       // length of value data 
    {
    }
	CString cnv = installPath + gameName + "Uninstall.exe";
    length = cnv.GetLength();
	if (RegSetValueEx(hk,             // subkey handle 
                        "UninstallString",       // value name 
                        0,                        // must be zero 
                        REG_EXPAND_SZ,            // value type 
                        (LPBYTE)cnv.GetBuffer(),           // pointer to value data 
                        length))       // length of value data 
    {
    }
	return 0;
}
//-----------------------------------------------------------------------------------------------------
void CCAppPatcherDlg::ChangeBackground(int resourceID)
{
    m_currentResource = resourceID;
    RedrawWindow();
}