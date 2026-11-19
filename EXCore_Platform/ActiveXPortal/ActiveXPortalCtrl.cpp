//ActiveXPortalCtrl.cpp : Implementation of the CActiveXPortalCtrl ActiveX Control class.

#include "stdafx.h"
#include "ActiveXPortal.h"
#include "ActiveXPortalCtrl.h"
#include "ActiveXPortalPropPage.h"
#include ".\activexportalctrl.h"
#include "directoryselect.h"
#include "winable.h"
#include "direct.h"
#include "getdxver.h"
#ifdef _DEBUG
#define new DEBUG_NEW
 #endif


IMPLEMENT_DYNCREATE(CActiveXPortalCtrl, COleControl)



//Message map

BEGIN_MESSAGE_MAP(CActiveXPortalCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()



//Dispatch map

BEGIN_DISPATCH_MAP(CActiveXPortalCtrl, COleControl)
    DISP_FUNCTION(CActiveXPortalCtrl, "SetUPG", SetUPG, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CActiveXPortalCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()



//Event map

BEGIN_EVENT_MAP(CActiveXPortalCtrl, COleControl)
END_EVENT_MAP()



//Property pages

//TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CActiveXPortalCtrl, 1)
	PROPPAGEID(CActiveXPortalPropPage::guid)
END_PROPPAGEIDS(CActiveXPortalCtrl)



//Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CActiveXPortalCtrl, "ACTIVEXPORTAL.ActiveXPortalCtrl.1",
	0x3c403675, 0xb43c, 0x410b, 0xbf, 0x56, 0xd4, 0xd1, 0xfb, 0x68, 0x35, 0x6c)



//Type library ID and version

IMPLEMENT_OLETYPELIB(CActiveXPortalCtrl, _tlid, _wVerMajor, _wVerMinor)



//Interface IDs

const IID BASED_CODE IID_DActiveXPortal =
		{ 0xDE6E43D5, 0x7B1B, 0x404F, { 0x95, 0x2A, 0x1B, 0xB7, 0xCD, 0x86, 0xC4, 0x5D } };
const IID BASED_CODE IID_DActiveXPortalEvents =
		{ 0x4E6AF5AC, 0xFB8E, 0x47AD, { 0xB6, 0x4A, 0x86, 0xF8, 0x91, 0x9D, 0x99, 0xFF } };



//Control type information

static const DWORD BASED_CODE _dwActiveXPortalOleMisc =
	OLEMISC_SIMPLEFRAME |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CActiveXPortalCtrl, IDS_ACTIVEXPORTAL, _dwActiveXPortalOleMisc)



//CActiveXPortalCtrl::CActiveXPortalCtrlFactory::UpdateRegistry -
//Adds or removes system registry entries for CActiveXPortalCtrl

BOOL CActiveXPortalCtrl::CActiveXPortalCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	//TODO: Verify that your control follows apartment-model threading rules.
	//Refer to MFC TechNote 64 for more information.
	//If your control does not conform to the apartment-model rules, then
	//you must modify the code below, changing the 6th parameter from
	//afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_ACTIVEXPORTAL,
			IDB_ACTIVEXPORTAL,
			afxRegApartmentThreading,
			_dwActiveXPortalOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}

DWORD WINAPI ValidateDownloadFiles(LPVOID lpParameter);
DWORD WINAPI RenderSpinner(LPVOID lpParameter);

//CActiveXPortalCtrl::CActiveXPortalCtrl - Constructor

CActiveXPortalCtrl::CActiveXPortalCtrl()
{
	m_mPlayer = NULL;
    m_maxTextureSize = 2048;
    m_shadowCount = 10;
	m_upgradeThreadWorking = FALSE;
	m_lgThreadHandle = NULL;
	m_engineClient = NULL;
	m_percentagePub = 0;
	m_bytesDownloaded = 0;
	m_toggleTest = 0;
	m_closeNow = 0;
	tested = FALSE;
	m_running = FALSE;
	m_rSp = FALSE;
	m_downloading = FALSE;
	m_openProgress = FALSE;
	//m_prgDlg = NULL;
    BOOL m_sup = FALSE;
    m_xEncrypt  =NULL;
    DWORD curVersion = 0x00090003;
    if(!DetectDirectXVersionReq(curVersion,"DirectX9c or better must be installed. Download DirectX from www.microsoft.com/directX."))
	    return;


	m_devMode = 0;
    m_ready = FALSE;
	m_axLogger.m_logFileName = "controlErrorLog.txt";
	m_encryptedMode = TRUE;
	 //m_axmedia           = NULL;
	m_pendingVolume  = 0;
	m_xEncrypt = new CAXEncryptionLib("qwertyuiopoiuytrewqwertyuiopoiuy");
	m_renderSpinner = FALSE;
	m_closeDown       = FALSE;
	m_disableMouseCmd = 0;
    m_rnThreadID = 0;
	m_rnThreadHandle = 0;
	m_downloadURL = "";
	m_startInFullscreen = FALSE;
	m_width				= 1024;
	m_height			= 768;
	m_depth				= 32;
	InitializeIIDs(&IID_DActiveXPortal, &IID_DActiveXPortalEvents);

     InitializeCriticalSection(&m_updateCriticalSection);
	
	EnableSimpleFrame();
	
	HKEY tempKey;
    
    m_installPath = abTool.GetSoftwareRegkeyVal("GAME_WORLDS","INSTALL_PATH");
    if(!SetCurrentDirectory(m_installPath))
        m_installPath = "";

	CString devMode = abTool.GetSoftwareRegkeyVal("GAME_WORLDS","DEV_ON");
	if(devMode.GetLength() > 0)
       m_devMode = 1;


    /*if(m_installPath.GetLength()==0)
	{//make user select path
	   m_axLogger.WriteDebugFile("Creating Content Path",0,1);
       CDirectorySelect dlg;
	   dlg.m_selectedDirectory = "C:\\GameWorlds\\";//set default//
	   dlg.DoModal();
	   m_axLogger.WriteDebugFile("Set Content Path:",0,0);
	   m_axLogger.WriteDebugFile(dlg.m_selectedDirectory.GetBuffer(),0,1);
	   m_installPath = dlg.m_selectedDirectory;
	   //abTool.MakeDirectory(m_installPath.GetBuffer());
	   CString tempDir = m_installPath+"CACHE\\";
	   abTool.MakeDirectory(tempDir.GetBuffer());
	   tempDir = m_installPath+"GameFiles\\";
	   abTool.MakeDirectory(tempDir.GetBuffer());
	   tempDir = m_installPath+"GameFiles\\";
	   abTool.MakeDirectory(tempDir.GetBuffer());
	   tempDir = m_installPath+"MapsModels\\";
	   abTool.MakeDirectory(tempDir.GetBuffer());
	   tempDir = m_installPath+"Sounds\\";
	   abTool.MakeDirectory(tempDir.GetBuffer());
	   abTool.SetSoftwareReg("GAME_WORLDS","INSTALL_PATH",m_installPath.GetBuffer());
	}//end make user select path*/
    m_contentDownloadManager = new CAXContentClient();
	m_axLogger.WriteDebugFile("Starting Up",1,1);
	m_axLogger.WriteDebugFile("Getting content Path",0,1);
	//aquire username and pass then set it
	m_cacheDirectory = m_installPath+"CACHE\\";
	abTool.MakeDirectory(m_cacheDirectory.GetBuffer());
	m_contentDownloadManager->SetCacheDirectory(m_cacheDirectory);
	m_axLogger.WriteDebugFile("Set Cache dir:",0,0);
	m_axLogger.WriteDebugFile(m_cacheDirectory.GetBuffer(),0,1);
}

//-----------------------------------------------------------//
bool CActiveXPortalCtrl::ReadUrl(CString & url, CString & retString)
{
     DWORD returnLength = 0;
	 char * buffer = NULL;
     CHttpFileDownloader * urlReaderTCPPort = new CHttpFileDownloader();
	 urlReaderTCPPort->m_asciMode = TRUE;
	 if(urlReaderTCPPort->GetUrlData("iexplorer data",url.GetBuffer(),(BYTE**)&buffer,&returnLength))
	 {
        retString = buffer;
        if(buffer)
           delete [] buffer;
		buffer = 0;
		delete urlReaderTCPPort;
	    urlReaderTCPPort = 0;
        return true;
	 }
	 delete urlReaderTCPPort;
	 urlReaderTCPPort = 0;
	 return false;
}
//-----------------------------------------------------------//
bool CActiveXPortalCtrl::ValidateFile(CString & fullPath, unsigned long fileCRC, BOOL checkCRC)
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
     if(!checkCRC)
        return true;

	 if(m_crcGenerator.getcrc(&fl)==fileCRC)
	 {//all good
        fl.Close();
		return true;
	 }//end all good
	 fl.Close();
     return false;
}
//-----------------------------------------------------------//
void CActiveXPortalCtrl::SetUPG(LPSTR username, 
								LPSTR password, 
								LPSTR game, 
								LPSTR downloadUrl, 
								LPSTR gserver,
								int fullScreen,
								int fullscreen_W,
								int fullscreen_H,
								int fullscreen_D)
{
	return;

	if(m_game.GetLength()>0)
	   return;

	m_username = username;
	m_username.Replace("[b]","\r\n");
	m_password = password;
	m_password.Replace("[b]","\r\n");
	m_game = game;
	m_game.Replace("[b]","\r\n");
	m_downloadURL = downloadUrl;
	m_downloadURL.Replace("[b]","\r\n");
    m_gserverIP = gserver;
	m_gserverIP.Replace("[b]","\r\n");

	if(m_game.GetLength() == 0)
	   return;

	m_axLogger.WriteDebugFile("SET UPG:",0,1);
    m_axLogger.WriteDebugFile(m_username.GetBuffer(),0,1);
	m_axLogger.WriteDebugFile(m_password.GetBuffer(),0,1);
	m_axLogger.WriteDebugFile(m_game.GetBuffer(),0,1);
	m_axLogger.WriteDebugFile(m_gserverIP.GetBuffer(),0,1);
	m_axLogger.WriteDebugFile(m_downloadURL.GetBuffer(),0,1);
	
	m_username		= m_xEncrypt->DecryptString64(m_username.GetBuffer());
	m_password		= m_xEncrypt->DecryptString64(m_password.GetBuffer());
	m_game			= m_xEncrypt->DecryptString64(m_game.GetBuffer());
	m_gserverIP     = m_xEncrypt->DecryptString64(m_gserverIP.GetBuffer());
	m_downloadURL   = m_xEncrypt->DecryptString64(m_downloadURL.GetBuffer());

	m_downloadURL = "http://" + m_downloadURL;

	m_axLogger.WriteDebugFile(m_username.GetBuffer(),0,1);
	m_axLogger.WriteDebugFile(m_password.GetBuffer(),0,1);
	m_axLogger.WriteDebugFile(m_game.GetBuffer(),0,1);
	m_axLogger.WriteDebugFile(m_gserverIP.GetBuffer(),0,1);
	m_axLogger.WriteDebugFile(m_downloadURL.GetBuffer(),0,1);

	m_startInFullscreen = fullScreen;
	m_width				= fullscreen_W;
	m_height			= fullscreen_H;
	m_depth				= fullscreen_D;

	EnterCriticalSection(&m_updateCriticalSection);
    m_ready = TRUE;
	LeaveCriticalSection(&m_updateCriticalSection);

}
//-----------------------------------------------------------//
bool CActiveXPortalCtrl::ParseUrlGetLine(int & curPos, CString & urlData, CString & lineString)
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
bool CActiveXPortalCtrl::ParseUrlStringToData(int set, 
											  CString urlParse, 
											  CString & fileName, 
											  long * fileSize, 
											  CString & cabName, 
											  CString & offsetDir, 
											  unsigned long * CRC,
											  unsigned long * cab_CRC,
											  BOOL * checkCrc)
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
		   *checkCrc		 = (BOOL)atoi(currentLineData.GetBuffer());//file CRC
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
//-----------------------------------------------------------//
bool CActiveXPortalCtrl::GetDataSetCount(int set)
{
    

	return true;
}
//-----------------------------------------------------------//
DWORD WINAPI ValidateDownloadFiles(LPVOID lpParameter)
{	   
	    
	    CActiveXPortalCtrl * classObj = (CActiveXPortalCtrl *)lpParameter; 
		if(FAILED(CoInitialize(NULL)))
		{
           return 0;
		}

		classObj->m_axLogger.WriteDebugFile("Check VS",0,1);

        EnterCriticalSection(&classObj->m_updateCriticalSection);
		classObj->m_upgradeThreadWorking = TRUE;
		while(classObj->m_ready == FALSE)
		{
   
		   LeaveCriticalSection(&classObj->m_updateCriticalSection);
		    Sleep(1000);
		   classObj->m_axLogger.WriteDebugFile("Waiting for Download URL:",0,1);
		   //ABORT
		   EnterCriticalSection(&classObj->m_updateCriticalSection);
		}
		LeaveCriticalSection(&classObj->m_updateCriticalSection);

		classObj->m_axLogger.WriteDebugFile("SET AUTH",0,1);
		//set authentication data
		classObj->m_contentDownloadManager->SetUsernamePassword(classObj->m_username,classObj->m_password);
		classObj->m_engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_userName = classObj->m_username;
		classObj->m_engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_passWord = classObj->m_password;
		classObj->m_engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_IPaddress = classObj->m_gserverIP;
		classObj->m_engineClient->m_initObject->m_gStatus->m_contentUrl = classObj->m_downloadURL;
        classObj->m_axLogger.WriteDebugFile("Set download url:",0,0);
		classObj->m_axLogger.WriteDebugFile(classObj->m_downloadURL.GetBuffer(),0,1);
		classObj->m_engineClient->m_initObject->m_gStatus->m_advUrl = classObj->m_downloadURL + "/contentmanager.aspx?adv=loadingimg";
		classObj->m_engineClient->m_initObject->m_gStatus->m_advUrlBasic = classObj->m_downloadURL + "/contentmanager.aspx?adv=basicimg";
	    classObj->m_engineClient->m_initObject->m_gStatus->m_advUrlVending = classObj->m_downloadURL + "/contentmanager.aspx?adv=vendimg";
		classObj->m_engineClient->m_initObject->m_gStatus->m_installPath = classObj->m_installPath;
		classObj->m_engineClient->m_initObject->m_gStatus->m_cacheDirectory = classObj->m_cacheDirectory;
		classObj->m_engineClient->m_initObject->m_gStatus->m_autoDownloadMissingAssets = TRUE;
		classObj->m_engineClient->m_initObject->m_gStatus->m_contentDownloadManager->SetUsernamePassword(classObj->m_username,classObj->m_password);
		classObj->m_engineClient->m_initObject->m_gStatus->m_contentDownloadManager->SetCacheDirectory(classObj->m_cacheDirectory);
        classObj->m_engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_ingameNameRef = classObj->m_game;
		classObj->m_axLogger.WriteDebugFile("Set Game Name Ref:",0,0);
		classObj->m_axLogger.WriteDebugFile(classObj->m_engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_ingameNameRef.GetBuffer(),0,1);

		classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeterAdv(75.0f,"Initializing...");
		//set Game specific data
	    SetCurrentDirectory(classObj->m_installPath);
		classObj->m_axLogger.WriteDebugFile(classObj->m_installPath.GetBuffer(),0,1);
		CString contentUrl = classObj->m_downloadURL + "/contentmanager.aspx?contenttype=init&gametype=" + classObj->m_game;
		CString ipTcpServer = "";
		CString tcpIPUrl   = classObj->m_downloadURL + "/contentmanager.aspx?tcpserver=1";
		CString tcpPortUrl = classObj->m_downloadURL + "/contentmanager.aspx?tcpserver=2";
		 long tcpPort = 0;
		
		CString temp = "";
		CString portString;
		if(classObj->ReadUrl(tcpIPUrl, ipTcpServer)==false)
		{//error
		   classObj->m_axLogger.WriteDebugFile(tcpIPUrl.GetBuffer(),0,0);
		   classObj->m_axLogger.WriteDebugFile(":FAILED ReadUrl() tcpIPUrl trying default",0,1);
		   classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeterAdv(100.0f,"tcpIPUrl READ FAILED:"+classObj->m_downloadURL);
		   ipTcpServer = "72.29.80.113";
		   //Sleep(1000);
		}//end error
		if(classObj->ReadUrl(tcpPortUrl, temp)==false)
		{//error
		   classObj->m_axLogger.WriteDebugFile(tcpPortUrl.GetBuffer(),0,0);
		   classObj->m_axLogger.WriteDebugFile(":FAILED ReadUrl() tcpPortUrl",0,1);
		   classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeterAdv(100.0f,"tcpPortUrl READ FAILED:"+classObj->m_downloadURL);
		   temp = "1050";
		   //Sleep(1000);
		}//end error
		portString = temp;
		tcpPort = atoi(temp);
		
		DWORD returnLength = 0;
		char * buffer = NULL;
		CHttpFileDownloader * urlReader = new CHttpFileDownloader();
		urlReader->m_asciMode = TRUE;
		if(urlReader->GetUrlData("iexplorer data",contentUrl.GetBuffer(),(BYTE**)&buffer,&returnLength))
		{//succeeded
		   classObj->m_axLogger.WriteDebugFile("GetUrlData() Success",0,1);
		CString startupFiles = buffer;
		if(buffer)
			delete [] buffer;
		buffer = 0;
	       
            int retryMax = 0;//0 = none
			

			CString	fileNameTemp = ""; 
			long		fileSizeTemp = 0;
			CString	cabNameTemp = "";
			CString	offsetDirTemp = ""; 
			unsigned long CRCTemp = 0;
			unsigned long CAB_CRCTemp = 0;
			int       currentSet = 0;
			BOOL      checkCrc = TRUE;
			BOOL      abort = FALSE;
			classObj->m_engineClient->m_initObject->m_gStatus->m_devMode = classObj->m_devMode;
			while(classObj->ParseUrlStringToData(currentSet, 
												 startupFiles,//CString urlParse, 
												 fileNameTemp, 
												 &fileSizeTemp, 
												 cabNameTemp, 
												 offsetDirTemp, 
												 &CRCTemp,
												 &CAB_CRCTemp,
												 &checkCrc))
			{//validate or download
				int retryCurrent = 0;
				CString fullFilePath = classObj->m_installPath + offsetDirTemp + fileNameTemp;
				classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeterAdv(75.0f,"Verifying:"+cabNameTemp);
				if(classObj->ValidateFile(fullFilePath, CRCTemp, checkCrc)==false && classObj->m_devMode == 0)
				{//file needs to be redownloaded
					
					classObj->m_contentDownloadManager->m_loggingSystem.WriteDebugFile("File verification Failed:",0,0);
					classObj->m_contentDownloadManager->m_loggingSystem.WriteDebugFile(fullFilePath.GetBuffer(),0,1);
					if(classObj->m_contentDownloadManager->DownloadFileRequest(cabNameTemp,ipTcpServer,tcpPort)==1)
					{//success to start thread for downloading
					DWORD timoutMsgStamp = GetTickCount();
					while(classObj->m_contentDownloadManager->GetPercentageOfDownload() < 100)
					{
						
							EnterCriticalSection(&classObj->m_updateCriticalSection);
                            if(classObj->m_contentDownloadManager->m_downloadInProgress == false)
							{
								if(GetTickCount()-timoutMsgStamp > 10000)
								{
								    classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeterAdv(100.0,classObj->m_contentDownloadManager->m_errorMessage);
							    
                                    abort = classObj->m_closeDown;
									if(abort)
									{
										classObj->m_upgradeThreadWorking = FALSE;
										LeaveCriticalSection(&classObj->m_updateCriticalSection);
										return 0;
									}
								}
							}
							else
							{
								classObj->m_percentagePub = classObj->m_contentDownloadManager->GetPercentageOfDownload();
								if(classObj->m_contentDownloadManager->m_errorMessage.GetLength()<2)
								{
								   CString kb_sec = classObj->m_engineClient->m_initObject->m_gStatus->m_cabLib.ConvToString(classObj->m_contentDownloadManager->GetKBSec()*8);
								   classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeterAdv((float)classObj->m_percentagePub,"kb: "+kb_sec+" Downloading:"+cabNameTemp);
								}
								else
								   classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeterAdv((float)classObj->m_percentagePub,classObj->m_contentDownloadManager->m_errorMessage+cabNameTemp);
								
					        }
							classObj->m_engineClient->m_initObject->m_gStatus->m_dxRenderer->EnableMouse();
							abort = classObj->m_closeDown;
							LeaveCriticalSection(&classObj->m_updateCriticalSection);

	                        if(abort == TRUE)
							{
							   EnterCriticalSection(&classObj->m_updateCriticalSection);
							   classObj->m_contentDownloadManager->Abort();
							   classObj->m_contentDownloadManager->WaitForThreadToClose();
							   classObj->m_upgradeThreadWorking = FALSE;
							   LeaveCriticalSection(&classObj->m_updateCriticalSection);
                               return 0;
							}
							Sleep(50);
					}
                    //Validate Download

					EnterCriticalSection(&classObj->m_updateCriticalSection);
					classObj->m_percentagePub = classObj->m_contentDownloadManager->GetPercentageOfDownload();
					classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeter((float)classObj->m_percentagePub);
					classObj->m_engineClient->m_initObject->m_gStatus->LoadSceneInfo();
					classObj->m_contentDownloadManager->Reset();
					int tryCount = 0;
					LeaveCriticalSection(&classObj->m_updateCriticalSection);
					BOOL resultFromDelete = 0;//DeleteFile(classObj->m_saveLocationWFile);//delete temp file
					//validate cabinet
					classObj->m_saveLocationWFile = classObj->m_cacheDirectory + cabNameTemp;
					if(classObj->ValidateFile(classObj->m_saveLocationWFile, CAB_CRCTemp, TRUE)==false && retryCurrent < retryMax)
					{//Cabinet failed
                        resultFromDelete = DeleteFile(classObj->m_saveLocationWFile);//delete temp file
						while(!resultFromDelete)
						{//delete failedfullFilePath
						classObj->m_contentDownloadManager->m_loggingSystem.WriteDebugFile(classObj->m_saveLocationWFile.GetBuffer(),0,0);
						classObj->m_contentDownloadManager->m_loggingSystem.WriteDebugFile("Delete Failed (CRC SECTION)!!",0,1);
						resultFromDelete = DeleteFile(classObj->m_saveLocationWFile);//delete temp file
						Sleep(50);
						tryCount++;
						if(tryCount > 10)
							break;
						}//end delete failed
                        currentSet--;
						retryCurrent++;
                       continue;
					}//end cabinet failed
					//extract cabinet to target directory


                
					//CCabinetObj newCabObj;
					CResourceCabinet newCabObj;
					newCabObj.Init();
					newCabObj.CreateFDIContext();
	                newCabObj.SetProgressCountRef(&classObj->m_percentagePub);
					newCabObj.SetCriticalSection(&classObj->m_updateCriticalSection);
					
					newCabObj.Copy(classObj->m_saveLocationWFile.GetBuffer(),classObj->m_installPath,NULL);
					Sleep(500);//let it digest
					tryCount = 0;
					//handle bad download
					
					classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeterAdv(97,"Verifying Download");
					if(classObj->ValidateFile(fullFilePath, CRCTemp, checkCrc)==false && classObj->m_devMode == 0 && retryCurrent < retryMax)
					{//file extracted badly
					   currentSet--;
					   classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeterAdv(100.0f,"File Corruption Error:"+fullFilePath);
                       resultFromDelete = DeleteFile(fullFilePath);
                       if(!resultFromDelete)
					   {
					       classObj->m_contentDownloadManager->m_loggingSystem.WriteDebugFile(fullFilePath.GetBuffer(),0,0);
                           classObj->m_contentDownloadManager->m_loggingSystem.WriteDebugFile("Delete Failed!!",0,1);
					   }
					}//end file extracted badly
					//clean up cabinet
					resultFromDelete = DeleteFile(classObj->m_saveLocationWFile);//delete temp file
					while(!resultFromDelete)
					{//delete failedfullFilePath
					   classObj->m_contentDownloadManager->m_loggingSystem.WriteDebugFile(classObj->m_saveLocationWFile.GetBuffer(),0,0);
                       classObj->m_contentDownloadManager->m_loggingSystem.WriteDebugFile("Delete Failed!!",0,1);
					   resultFromDelete = DeleteFile(classObj->m_saveLocationWFile);//delete temp file
					   Sleep(50);
					   tryCount++;
					   if(tryCount > 10)
					      break;
					}//end delete failed
					
					}//end success to start thread for downloading
				}//end file needs to be redownloaded
				currentSet++;
			}//end validate or download
		}//end succeded
		else
		{
			classObj->m_axLogger.WriteDebugFile("Cannot connect to content Server:",0,0);
			classObj->m_axLogger.WriteDebugFile(contentUrl.GetBuffer(),0,1);
			
			classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeterAdv(100.0f,"Cannot connect to content Server  GAME:"+classObj->m_game);
			Sleep(1000);
		}
	    
		delete urlReader;
		urlReader = 0;
		//load game specific data

		//reload language filter
        if(classObj->m_engineClient->m_initObject->m_gStatus->m_langFilter)
		   delete classObj->m_engineClient->m_initObject->m_gStatus->m_langFilter;
		classObj->m_engineClient->m_initObject->m_gStatus->m_langFilter  = 0;

		classObj->m_engineClient->m_initObject->m_gStatus->m_langFilter = new CAXLanguageFilter();

		classObj->m_axLogger.WriteDebugFile("Pre Run Script()",0,1);
		classObj->m_engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_currentVersion = 252;
        classObj->m_engineClient->m_initObject->m_gStatus->RunScript(classObj->m_installPath + "GameFiles\\" + classObj->m_game + ".aut");//load from scripts
		//end load game specific data
		EnterCriticalSection(&classObj->m_updateCriticalSection);
		classObj->m_rSp = FALSE;
	    LeaveCriticalSection(&classObj->m_updateCriticalSection);

		char bufferXY[_MAX_PATH];
	    _getcwd( bufferXY, _MAX_PATH );
        classObj->m_axLogger.WriteDebugFile(bufferXY,0,1);
 
	    classObj->m_axLogger.WriteDebugFile("Trying to connect....",0,1);
        //create render thread
	
		classObj->m_rnThreadHandle = CreateThread(NULL,0,RenderSpinner,classObj,0,&classObj->m_rnThreadID);//call validate/dowloader

		EnterCriticalSection(&classObj->m_updateCriticalSection);
		classObj->m_upgradeThreadWorking = FALSE;
		LeaveCriticalSection(&classObj->m_updateCriticalSection);

	return 1;
}



//CActiveXPortalCtrl::~CActiveXPortalCtrl - Destructor

CActiveXPortalCtrl::~CActiveXPortalCtrl()
{

    if(!m_xEncrypt)
		return;

	if(m_engineClient)
	   m_engineClient->m_initObject->m_gStatus->SetCloseDownCallState(TRUE);
    EnterCriticalSection(&m_updateCriticalSection);
	m_closeDown = TRUE;
	LeaveCriticalSection(&m_updateCriticalSection);
	Sleep(100);
	EnterCriticalSection(&m_updateCriticalSection);
	if(m_lgThreadHandle != NULL)
	   CloseHandle(m_lgThreadHandle);
	
	
	if(m_renderSpinner == TRUE)
	while(m_closeDown == TRUE)
	{
      LeaveCriticalSection(&m_updateCriticalSection);
      Sleep(100);
	  EnterCriticalSection(&m_updateCriticalSection);
	}
	LeaveCriticalSection(&m_updateCriticalSection);
	
	EnterCriticalSection(&m_updateCriticalSection);
	while(m_upgradeThreadWorking == TRUE)
	{
      LeaveCriticalSection(&m_updateCriticalSection);
      Sleep(100);
	  EnterCriticalSection(&m_updateCriticalSection);
	}
	LeaveCriticalSection(&m_updateCriticalSection);
	   

	if(m_contentDownloadManager)
       delete m_contentDownloadManager;
	   m_contentDownloadManager = 0;

	if(m_engineClient)
       delete m_engineClient;
	   m_engineClient = 0;

    if(m_xEncrypt)
	   delete m_xEncrypt;
	m_xEncrypt = 0;


	 DeleteCriticalSection(&m_updateCriticalSection);
}



//CActiveXPortalCtrl::OnDraw - Drawing function

void CActiveXPortalCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;
}



//CActiveXPortalCtrl::DoPropExchange - Persistence support

void CActiveXPortalCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

    if(!m_xEncrypt)
	    return;

    if(m_game.GetLength()>0)
	   return;

	//return;

    m_axLogger.WriteDebugFile("DoPropExchange()",0,1);

	CString temp = "";
	short tempShort = 0;

    

    if(PX_Short(pPX, _T("ENCRYPTED"), tempShort))
	{
		if(tempShort == 1)
		   m_encryptedMode = 0;
	}

	 if(PX_String(pPX, _T("USERNAME"), temp))
	{
		if(temp.GetLength() > 2)
		{
			if(m_encryptedMode)
			{
			  temp.Replace("[b]","\r\n");
			  if(temp.GetLength() > 1)
			     m_username = m_xEncrypt->DecryptString64(temp.GetBuffer());
			}
			else
			     m_username = temp;
		}
	}

	if(PX_String(pPX, _T("PASSWORD"), temp))
	{
		if(temp.GetLength() > 2)
		{
			if(m_encryptedMode)
			{
			temp.Replace("[b]","\r\n");
			if(temp.GetLength() > 1)
			   m_password = m_xEncrypt->DecryptString64(temp.GetBuffer());
			}
			else
			m_password = temp;
		}
	}

	if(PX_String(pPX, _T("GAME"), temp))
	{
        if(temp.GetLength() > 2)
		{

			if(m_encryptedMode)
			{
			temp.Replace("[b]","\r\n");
			if(temp.GetLength() > 1)
			m_game = m_xEncrypt->DecryptString64(temp.GetBuffer());
			}
			else
			m_game = temp;

			m_axLogger.WriteDebugFile("SET GAME:",0,0);
			m_axLogger.WriteDebugFile(m_game.GetBuffer(),0,1);
		}
	}

	if(PX_String(pPX, _T("DOWNLOADURL"), temp))
	{
        if(temp.GetLength() > 2)
		{

			m_axLogger.WriteDebugFile("ENC URL:",0,0);
			m_axLogger.WriteDebugFile(temp.GetBuffer(),0,1);
			if(m_encryptedMode)
			{
				temp.Replace("[b]","\r\n");
				if(temp.GetLength() > 1)
				{
				m_downloadURL = m_xEncrypt->DecryptString64(temp.GetBuffer());
				m_downloadURL = "http://" + m_downloadURL;
				}
			}
			else
			{
				m_downloadURL = temp;
				m_downloadURL = "http://" + m_downloadURL;
			}
		}
	}

	if(PX_String(pPX, _T("GAMESERVER"), temp))
	{
        if(temp.GetLength() > 2)
		{

			m_axLogger.WriteDebugFile("Game Server IP:",0,0);
			m_axLogger.WriteDebugFile(temp.GetBuffer(),0,1);
			if(m_encryptedMode)
			{
			temp.Replace("[b]","\r\n");
			if(temp.GetLength() > 1)
				m_gserverIP = m_xEncrypt->DecryptString64(temp.GetBuffer());
			}
			else
			{
			m_gserverIP = temp;
			}
		}
	}

    if(PX_String(pPX,_T("FULLSCREEN"),temp))
	{
	   if(temp.CompareNoCase("on")==0)
          m_startInFullscreen = 1;

	    m_axLogger.WriteDebugFile("SET FULLSCREEN:",0,0);
		m_axLogger.WriteDebugFile(temp.GetBuffer(),0,1);
	}

	if(PX_String(pPX,_T("MAXTEX"),temp))
	{
	   if(temp.GetLength() > 0)
	   {
        m_maxTextureSize = atoi(temp.GetBuffer());
	    m_axLogger.WriteDebugFile("SET MAX TEXTURE SIZE:",0,0);
		m_axLogger.WriteDebugFile(temp.GetBuffer(),0,1);
	   }
	}

	if(PX_String(pPX,_T("SHADOWS"),temp))
	{
	   if(temp.GetLength() > 0)
	   {
        m_shadowCount = atoi(temp.GetBuffer());
	    m_axLogger.WriteDebugFile("SHADOW COUNT:",0,0);
		m_axLogger.WriteDebugFile(temp.GetBuffer(),0,1);
	   }
	}

	
	if(PX_Short(pPX,_T("FULLSCREEN_W"),tempShort))
	{
	   if(tempShort > 0)
          m_width = tempShort;
	}

	if(PX_Short(pPX,_T("FULLSCREEN_H"),tempShort))
	{
	    if(tempShort > 0)
           m_height = tempShort;
	}

	if(PX_Short(pPX,_T("FULLSCREEN_D"),tempShort))
	{
	    if(tempShort > 0)
           m_depth = tempShort;
	}

	if(m_game.GetLength()==0)
	   return;

	EnterCriticalSection(&m_updateCriticalSection);
    m_ready = TRUE;
	LeaveCriticalSection(&m_updateCriticalSection);
}



//CActiveXPortalCtrl::OnResetState - Reset control to default state

void CActiveXPortalCtrl::OnResetState()
{
	COleControl::OnResetState();  //Resets defaults found in DoPropExchange

	//TODO: Reset any other control state here.
}



//CActiveXPortalCtrl::AboutBox - Display an "About" box to the user

void CActiveXPortalCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_ACTIVEXPORTAL);
	dlgAbout.DoModal();
}



//CActiveXPortalCtrl message handlers
//-----------------------------------------------------------//
DWORD WINAPI RenderSpinner(LPVOID lpParameter)
{	   
	    CActiveXPortalCtrl * classObj = (CActiveXPortalCtrl *)lpParameter; 
		if(FAILED(CoInitialize(NULL)))
		{
           return 0;
		}
		classObj->m_engineClient->m_initObject->m_gStatus->m_dxRenderer->EnableManagerCallback();
		classObj->m_engineClient->m_initObject->m_gStatus->m_dxRenderer->DisableMouse();
 
		classObj->m_renderSpinner = TRUE;
		while(1)
		{
		   
           EnterCriticalSection(&classObj->m_updateCriticalSection);
	       if(classObj->m_closeDown == TRUE)
	       {
		      classObj->m_closeDown = FALSE;
              LeaveCriticalSection(&classObj->m_updateCriticalSection);
              break;   
	       }
	       LeaveCriticalSection(&classObj->m_updateCriticalSection);

           classObj->m_engineClient->CallBackTimeOpen();
		   //classObj->m_engineClient->m_initObject->m_gStatus->m_dxRenderer->MouseManagerCallback();
		   classObj->m_engineClient->ClientCallback();
		   classObj->m_engineClient->CallBackTimeCap();

		   if(classObj->m_engineClient->m_initObject->m_gStatus->GetCloseDownCallState())
	          ::PostMessage(classObj->m_engineClient->m_initObject->m_gStatus->m_dxRenderer->m_topWindowHandle, WM_CLOSE, 0, 0 );
		}

		

		return 0;
}


void CActiveXPortalCtrl::OnTimer(UINT nIDEvent)
{
	if(!m_engineClient)
		return;

	m_engineClient->m_initObject->m_userControls->MouseManagerCallback();
	

           if(m_PlayerEnabled)
		   {//player enabled
		    EnterCriticalSection(&m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_mSection);
			m_pendingUrl = m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_pendingURL;
			m_pendingVolume = m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_currentVolume;
			LeaveCriticalSection(&m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_mSection);
		    if(m_pendingUrl.GetLength() > 0)
			{//streaming music time
				if(m_pendingUrl.CompareNoCase("stop")==0)
				Stop();
				else
				{
					CString descBuffer;
					CString descUrl = m_pendingUrl + "&desc=1";
					if(ReadUrl(descUrl, descBuffer)==true)
					{
                       m_engineClient->m_initObject->m_gStatus->m_systemReadout->AddLine(descBuffer,MENU_TEXTCOLOR_NORMAL);
					}
					if(!PlayURL(m_pendingUrl.GetBuffer()))
					{
                        m_PlayerEnabled = FALSE;
						m_pendingUrl = "";
						return;
					}
					/*
					m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->SetVolume(0);
					m_pendingVolume = 0;
					*/
				}
				//handle volume
				EnterCriticalSection(&m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_mSection);
				m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_currentURL =  m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_pendingURL;
				m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_pendingURL = "";
				m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->SetMusicStreamerEnable(TRUE);
				LeaveCriticalSection(&m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_mSection);
			}//end streaming music time

			
			if(m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->GetMusicStreamerStatus())
			{
				if(m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->GetDisabledStatus() != m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->GetCurDisabledStateStatus())
				{
					Stop();
					m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_trackInUse = -1;
					//m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_disabledCurState = m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_disabled;
				    m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->SetDisabledCurState(m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->GetDisabledStatus());
				}

				if(m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->VolumeUpdateNeeded())
				{
					    long volume = m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->GetVolume();
						SetVolume(volume);
						m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->UpdateLastSetVolume(volume);
				}
			}
			
			
		   }//end player enabled

	       /*if(classObj->m_PlayerEnabled)
		   {
		    if(classObj->m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_pendingURL.GetLength() > 0)
			{//streaming music time
				if(classObj->m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_pendingURL.CompareNoCase("stop")==0)
					classObj->Stop();
				else
				{
					classObj->PlayURL(classObj->m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_pendingURL.GetBuffer());
					classObj->m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_currentVolume = 0;
					//m_gStatus->m_systemReadout->AddLine("Now Playing "+m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_stationName,.2,.2,.8);
					classObj->m_engineClient->m_initObject->m_gStatus->m_systemReadout->AddLine("Now Playing "+classObj->WhatsPlaying(),.2,.2,.8);
					
				}
				//handle volume
				
				classObj->m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_currentURL =  classObj->m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_pendingURL;
				classObj->m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_pendingURL = "";

				classObj->m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_musicStreamerEnabled = TRUE;
			}//end streaming music time

			if(classObj->m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_musicStreamerEnabled)
			{
				if(classObj->m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_disabled != classObj->m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_disabledCurState)
				{
					classObj->Stop();
					classObj->m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_trackInUse = -1;
					classObj->m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_disabledCurState = classObj->m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_disabled;
				}

				if(classObj->m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_currentTargetVolume != classObj->m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_lastSetVolume)
				{
						classObj->SetVolume(classObj->m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_currentTargetVolume);
						classObj->m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_lastSetVolume = classObj->m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_currentTargetVolume;
				}
			}
		   }*/

	COleControl::OnTimer(nIDEvent);
}

void CActiveXPortalCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	COleControl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CActiveXPortalCtrl::OnKeyPressEvent(USHORT nChar)
{

	COleControl::OnKeyPressEvent(nChar);
}

CString CActiveXPortalCtrl::ConvToString(int number)
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

void CActiveXPortalCtrl::OnDestroy()
{
	COleControl::OnDestroy();

	
	 //TODO: Add your message handler code here
}

int CActiveXPortalCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	//Init engine
    if(!m_xEncrypt)
	    return 0;
	
	
	HWND windowHandleTest = NULL;
		windowHandleTest = GetSafeHwnd();
		SetCurrentDirectory(m_installPath.GetBuffer());
		if(windowHandleTest)
		if(!m_running)
		if(!m_engineClient)
		{
			m_axLogger.WriteDebugFile("Init Client Object",0,1);
			windowHandle = GetSafeHwnd();
			HWND fullScreenWindow = GetTopmostParent();
			::GetWindowRect(windowHandleTest,&m_windowSz);

			m_PlayerEnabled = FALSE;
            if(abTool.ValidateRegistryEntry("Software\\Microsoft\\Active Setup\\Installed Components\\{6BF52A52-394A-11d3-B153-00C04F79FAA6}"))
			{//make sure control exists on machine
                if(!m_mPlayer)
				    m_mPlayer = new COcx2();

				BOOL resCnt = m_mPlayer->Create(NULL, 0, CRect(50,50,100,100), this, 0);
				if(resCnt)
				{
					if(VersionToNumber(m_mPlayer->get_versionInfo()) > 8)
					   m_PlayerEnabled = TRUE;
				}
			}//end make sure control exists on machine
			if(!m_startInFullscreen)
			    m_engineClient = new CClientModule(windowHandle,0,0,(m_windowSz.right-m_windowSz.left),(m_windowSz.bottom-m_windowSz.top),32,FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,m_maxTextureSize,m_shadowCount,FALSE);
			else
				m_engineClient = new CClientModule(windowHandle,0,0,m_width,m_height,m_depth,TRUE,FALSE,TRUE,TRUE,TRUE,TRUE,m_maxTextureSize,m_shadowCount,FALSE);
			

			if(m_engineClient->m_initObject->m_gStatus)
			{
				m_axLogger.WriteDebugFile("Init Client Complete",0,1);
				m_running = TRUE;
				DWORD threadID = 0;
				m_rSp = TRUE;
				SetTimer(1,100,0);
				m_lgThreadHandle = CreateThread(NULL,0,ValidateDownloadFiles,this,0,&threadID);//call validate/dowloader
			}
		}
		m_axLogger.WriteDebugFile("Init Client Exit",0,1);

	return 0;
}

int CActiveXPortalCtrl::VersionToNumber(CString stringVer)
{
    int length = stringVer.GetLength();
    if(length == 0)
	   return 0;

    CString build = "";
	for(int loop=0;loop<length;loop++)
	{
        if(stringVer.GetAt(loop) == '.')
		   break;

		build += stringVer.GetAt(loop);
	}

    return atoi(build);
}

void CActiveXPortalCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	//TODO: Add your message handler code here and/or call default

	COleControl::OnMouseMove(nFlags, point);
}

HWND CActiveXPortalCtrl::GetTopmostParent()
{
    HWND current = GetSafeHwnd();
	HWND topMost = ::GetParent(current);
	if(topMost == NULL)
	   return current;
 
	while(topMost != NULL)
	{
       current = topMost;
       topMost = ::GetParent(current);
	}
    return current;
}

void CActiveXPortalCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_engineClient)
	   m_engineClient->m_initObject->m_gStatus->m_dxRenderer->HideMouseByRectClick();

	return;

	COleControl::OnLButtonDown(nFlags, point);
}

void CActiveXPortalCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	return;

	COleControl::OnRButtonDown(nFlags, point);
}


BOOL CActiveXPortalCtrl::Stop()
{
    if(!m_mPlayer)
		return FALSE;

	IWMPControls * controls = (IWMPControls *)m_mPlayer->get_controls();
	if(controls)
	   controls->stop();
	return TRUE;
}

BOOL CActiveXPortalCtrl::PlayURL(char * url)
{
    if(!m_mPlayer)
		return FALSE;

	Stop();
	LPDISPATCH dispatch = m_mPlayer->newMedia(url);
	if(dispatch)
	{
	   m_mPlayer->put_currentMedia(dispatch);
	   IWMPControls * controls = (IWMPControls *)m_mPlayer->get_controls();
	   if(controls)
	      controls->play();
	   return TRUE;
	}
	return FALSE;
}

BOOL CActiveXPortalCtrl::SetVolume(int volume)
{
	
	//IWMPControls * controls = (IWMPControls *)m_mPlayer.get_controls();
	IWMPSettings * settings = (IWMPSettings *)m_mPlayer->get_settings();
	if(settings)
	{
	   HRESULT errorRes = settings->put_volume(volume);
	   return TRUE;
	}
	return FALSE;
}

CString CActiveXPortalCtrl::WhatsPlaying()
{
    /*IWMPMedia * media = (IWMPMedia *)m_mPlayer.get_currentMedia();
	if(media)
	{
	long cnt = 0;
    media->get_attributeCount(&cnt);

	for(long i=0;i<cnt;i++)
	{
		BSTR str;
		media->getItemInfoByAtom(i,&str);
		CString test = (LPSTR)str;
	}
	

	
    return "";
	}*/
	return "error 233";
}

void CActiveXPortalCtrl::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	COleControl::OnActivate(nState, pWndOther, bMinimized);

	if(m_running)
	{
		if (WA_INACTIVE == nState)
		{
	        m_engineClient->m_initObject->m_userControls->UnacquireInputDevices();
		}
		else
		{
	        m_engineClient->m_initObject->m_userControls->AcquireInputDevices();
		}
	}

    //Set exclusive mode access to the mouse based on active state
    //SetAcquire();

}

//------------------Direct X Version detection---------------------//
BOOL CActiveXPortalCtrl::DetectDirectXVersionReq(DWORD minVersion,
													CString errorMsg)
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
        //   requirementMet = TRUE;
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
