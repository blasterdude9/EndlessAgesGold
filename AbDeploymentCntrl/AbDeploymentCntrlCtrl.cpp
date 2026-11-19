// AbDeploymentCntrlCtrl.cpp : Implementation of the CAbDeploymentCntrlCtrl ActiveX Control class.

#include "stdafx.h"
#include <afxinet.h>
#include "AbDeploymentCntrl.h"
#include "AbDeploymentCntrlCtrl.h"
#include "AbDeploymentCntrlPropPage.h"
#include ".\abdeploymentcntrlctrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CAbDeploymentCntrlCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CAbDeploymentCntrlCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CAbDeploymentCntrlCtrl, COleControl)
	DISP_FUNCTION_ID(CAbDeploymentCntrlCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION(CAbDeploymentCntrlCtrl, "DownloadFileToDisk", DownloadFileToDisk, VT_I4, VTS_BSTR VTS_BSTR VTS_BOOL)
    DISP_FUNCTION(CAbDeploymentCntrlCtrl, "ExtractCabFile", ExtractCabFile, VT_I2, VTS_BSTR VTS_BSTR VTS_BOOL)
    DISP_FUNCTION(CAbDeploymentCntrlCtrl, "SetSoftwareRegistry", SetSoftwareRegistry, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR)
    DISP_FUNCTION(CAbDeploymentCntrlCtrl, "GetSoftwareRegkeyValue", GetSoftwareRegkeyValue, VT_BSTR, VTS_BSTR VTS_BSTR)
    DISP_FUNCTION(CAbDeploymentCntrlCtrl, "LaunchExecutable", LaunchExecutable, VT_EMPTY, VTS_BSTR VTS_BSTR)
    DISP_FUNCTION(CAbDeploymentCntrlCtrl, "MakeDirectory", MakeDirectory, VT_EMPTY, VTS_BSTR)
    DISP_FUNCTION(CAbDeploymentCntrlCtrl, "GetFreeSpaceMB", GetFreeSpaceMB, VT_I4, VTS_BSTR)
    DISP_FUNCTION(CAbDeploymentCntrlCtrl, "ValidateVersion", ValidateVersion, VT_I4, VTS_BSTR)
    DISP_FUNCTION(CAbDeploymentCntrlCtrl, "WriteVersion", WriteVersion, VT_EMPTY, VTS_BSTR VTS_I4)
	DISP_FUNCTION(CAbDeploymentCntrlCtrl, "StartGame", StartGame, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CAbDeploymentCntrlCtrl, COleControl)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CAbDeploymentCntrlCtrl, 1)
	PROPPAGEID(CAbDeploymentCntrlPropPage::guid)
END_PROPPAGEIDS(CAbDeploymentCntrlCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CAbDeploymentCntrlCtrl, "ABDEPLOYMENTCNTR.AbDeploymentCntrCtrl.1",
	0x31cfd255, 0x1408, 0x4782, 0x83, 0x4e, 0xc9, 0xf9, 0xc9, 0xd0, 0x77, 0xc6)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CAbDeploymentCntrlCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID BASED_CODE IID_DAbDeploymentCntrl =
		{ 0x1C678D41, 0xD494, 0x487C, { 0xB2, 0x2, 0xDE, 0xFE, 0xCC, 0x25, 0x35, 0xD2 } };
const IID BASED_CODE IID_DAbDeploymentCntrlEvents =
		{ 0x67C753D9, 0x6EB2, 0x4F15, { 0x8A, 0x39, 0xF7, 0x5E, 0x80, 0x1C, 0x24, 0xFE } };



// Control type information

static const DWORD BASED_CODE _dwAbDeploymentCntrlOleMisc =
	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CAbDeploymentCntrlCtrl, IDS_ABDEPLOYMENTCNTRL, _dwAbDeploymentCntrlOleMisc)

DWORD WINAPI RenderLoopThread(LPVOID lpParameter);

// CAbDeploymentCntrlCtrl::CAbDeploymentCntrlCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CAbDeploymentCntrlCtrl

BOOL CAbDeploymentCntrlCtrl::CAbDeploymentCntrlCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_ABDEPLOYMENTCNTRL,
			IDB_ABDEPLOYMENTCNTRL,
			afxRegApartmentThreading,
			_dwAbDeploymentCntrlOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CAbDeploymentCntrlCtrl::CAbDeploymentCntrlCtrl - Constructor

CAbDeploymentCntrlCtrl::CAbDeploymentCntrlCtrl()
{
	InitializeIIDs(&IID_DAbDeploymentCntrl, &IID_DAbDeploymentCntrlEvents);
	// TODO: Initialize your control's instance data here.

	//m_dxDeviceObj = NULL;
	m_engineClient = NULL;
	m_globalRenderLoopThreadID = 0;
	m_running                  = FALSE;
	m_rSp                      = FALSE;
	m_test                     = FALSE;
	//windowHandle;
    

}



// CAbDeploymentCntrlCtrl::~CAbDeploymentCntrlCtrl - Destructor

CAbDeploymentCntrlCtrl::~CAbDeploymentCntrlCtrl()
{
	/*if(m_dxDeviceObj)
	   delete m_dxDeviceObj;
	m_dxDeviceObj = 0;*/
	if(m_engineClient)
	   delete m_engineClient;
	   m_engineClient = 0;

	   
}



// CAbDeploymentCntrlCtrl::OnDraw - Drawing function

void CAbDeploymentCntrlCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

    if(!m_test)
    AfxMessageBox("test 432");

	m_test = TRUE;
    /*SetCurrentDirectory("C:\\Program Files\\Endless Ages Gold\\Endless Ages Gold\\Endless Ages Gold\\");

	if(!m_running)
	if(!m_engineClient)
	{
		m_running = TRUE;
	    m_engineClient = new CClientModule(windowHandle,500,400,32);
		//m_globalRenderLoopHandle = CreateThread(NULL,0,RenderLoopThread,m_engineClient,0,&m_globalRenderLoopThreadID);
		m_engineClient->CallBackTimeOpen();
	    m_engineClient->ClientCallback();
	    m_engineClient->CallBackTimeCap();
		SetTimer(1, 20, 0);

	}*/

	
	

	// TODO: Replace the following code with your own drawing code.
	//pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	//pdc->Ellipse(rcBounds);

    


}



// CAbDeploymentCntrlCtrl::DoPropExchange - Persistence support

void CAbDeploymentCntrlCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CAbDeploymentCntrlCtrl::OnResetState - Reset control to default state

void CAbDeploymentCntrlCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CAbDeploymentCntrlCtrl::AboutBox - Display an "About" box to the user

void CAbDeploymentCntrlCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_ABDEPLOYMENTCNTRL);
	dlgAbout.DoModal();
}



// CAbDeploymentCntrlCtrl message handlers

void CAbDeploymentCntrlCtrl::StartGame()
{
    SetCurrentDirectory("C:\\Program Files\\Endless Ages Gold\\Endless Ages Gold\\Endless Ages Gold\\");

	if(!m_running)
	if(!m_engineClient)
	{
		m_running = TRUE;
		windowHandle = GetSafeHwnd();
	    m_engineClient = new CClientModule(windowHandle,500,400,32);
		//m_globalRenderLoopHandle = CreateThread(NULL,0,RenderLoopThread,m_engineClient,0,&m_globalRenderLoopThreadID);
		m_engineClient->CallBackTimeOpen();
	    m_engineClient->ClientCallback();
	    m_engineClient->CallBackTimeCap();
		SetTimer(1, 20, 0);

	}
	return;
	//end test
}


void CAbDeploymentCntrlCtrl::LaunchExecutable(LPSTR path, LPSTR fileNameX)
{
    StartGame();
	return;
	//Test above
    

    CString fileName = fileNameX;

	if(fileName.GetLength() < 1)
	   return;
	SetCurrentDirectory(path);
	
	//char *args[4];
	
	char prog[256];// = new char[fileName.GetLength()+1];
    ZeroMemory(prog,256);
 
	for(int i=0;i<fileName.GetLength();i++)
        prog[i] = (char)fileName.GetAt(i);

	_spawnl( _P_WAIT, prog, "exec??", "_spawnl", "two", NULL );
}

long CAbDeploymentCntrlCtrl::DownloadFileToDisk(LPSTR url,
										        LPSTR localFilePathName,
										        BOOL showProgressMeter)
{

  //CString url,
  //CString localFilePathName
  CInternetSession session("Control Session");
  CStdioFile* pFile = NULL;
  //use a URL and display a Web page
  pFile = session.OpenURL(url,
	                      1,
	                      INTERNET_FLAG_TRANSFER_BINARY,
						  NULL,
						  NULL);
  if(!pFile)
      return 0;//error no file found

   long bytes = (long)pFile->SeekToEnd();
   pFile->SeekToBegin();

   HANDLE hFile = CreateFile(localFilePathName,
         GENERIC_WRITE, FILE_SHARE_READ,
         NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

   if (hFile == INVALID_HANDLE_VALUE)
   {
       //parse directory
       MakeDirectory(localFilePathName);

	   hFile = CreateFile(localFilePathName,
         GENERIC_WRITE, FILE_SHARE_READ,
         NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
   }
  

  
   if (hFile == INVALID_HANDLE_VALUE)
   {
      
	   
	   
	  MessageBox("Write error 3045");
      return 0;
   }

   BYTE szBuff[1025];
   CFile testGenFile(hFile);

   int dataSz = 1;
   int traceProgress = 0;
   int totalProgLength = bytes/1024;
   CDialog * progressInternal = new CDialog;
   if(showProgressMeter)
   {
     progressInternal->Create(IDD_DIALOG_PROGRESS,NULL);
     progressInternal->ShowWindow(SW_SHOW);
     progressInternal->SendDlgItemMessage(IDC_PROGRESS_METER,PBM_SETRANGE ,0,MAKELPARAM(0,totalProgLength));
     progressInternal->SendDlgItemMessage(IDC_PROGRESS_METER,PBM_SETBARCOLOR,0,RGB(0,100,0));
   }
   int totalBytesDownloaded = 0;
   while (dataSz > 0)
   {
      dataSz = pFile->Read(szBuff,1024);
      testGenFile.Write( szBuff,dataSz);

	  if(showProgressMeter)
	  {
		 totalBytesDownloaded += 1024;
	     progressInternal->SetDlgItemInt(IDC_STATIC_Status,(totalBytesDownloaded/1000),TRUE);//Show KB downloaded progress
	     progressInternal->SendDlgItemMessage(IDC_PROGRESS_METER,PBM_SETPOS,traceProgress,0);
	     progressInternal->UpdateData(TRUE);
	  }
	  traceProgress++;
   }
   testGenFile.Close();
   delete pFile;
   session.Close();
   delete progressInternal;
   progressInternal = 0;
   return 1;
}

int CAbDeploymentCntrlCtrl::ExtractCabFile(LPSTR destPath,LPSTR CabinetFile,BOOL showProgress)
{
CCabinetObj * cabConrolObj = new CCabinetObj();
int err = 0;
MakeDirectory(destPath);
err = cabConrolObj->ExtractCabinetToDirectory(destPath,CabinetFile);


delete cabConrolObj;
cabConrolObj = 0;

return err;
}


LPSTR CAbDeploymentCntrlCtrl::GetSoftwareRegkeyValue(LPSTR headerX,LPSTR valueRequest)
{
	CString header = headerX;
            HKEY hKey;
			int BUFSIZE = 500;
            char szProductType[500];
			ZeroMemory(szProductType,BUFSIZE);
            DWORD dwBufLen=BUFSIZE;
            LONG lRet;

            lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                                 "SOFTWARE\\"+header,
                                 0, 
								 KEY_QUERY_VALUE, 
								 &hKey );
            
			if( lRet != ERROR_SUCCESS )
               return "";

            lRet = RegQueryValueEx( hKey, valueRequest, NULL, NULL,
                                 (LPBYTE) szProductType, &dwBufLen);
            if( (lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE) )
               return "";

            RegCloseKey( hKey );

            return szProductType;
}

void CAbDeploymentCntrlCtrl::SetSoftwareRegistry(LPSTR headerX,
											LPSTR valueName,
											LPSTR valueDataX)
{
   HKEY hk; 
   DWORD dwData; 
   CString valueData = valueDataX;
   CString header = headerX;

   if(RegCreateKey(HKEY_LOCAL_MACHINE, 
                   "SOFTWARE\\"+header,
				   &hk))
   {
        
   }

   if(valueData.GetLength() < 1)
      return;

   char * szBuf = new char[valueData.GetLength()+1];
   ZeroMemory(szBuf,valueData.GetLength()+1);

   for(int i=0;i<valueData.GetLength();i++)
   {
       szBuf[i] = valueData.GetAt(i);
   }

   if (RegSetValueEx(hk,             // subkey handle 
                     valueName,       // value name 
                     0,                        // must be zero 
                     REG_EXPAND_SZ,            // value type 
                     (LPBYTE) szBuf,           // pointer to value data 
                     strlen(szBuf) + 1))       // length of value data 
   {

   }


   delete [] szBuf;
   szBuf = 0;

}

long CAbDeploymentCntrlCtrl::GetFreeSpaceMB(LPSTR path)
{
       
	DWORD sectorsPerCluster, bytesPerSector, numberOfFreeClusters, totalNumberOfClusters;
	
	
	GetDiskFreeSpace(path,
         (DWORD*)&sectorsPerCluster,
         (DWORD*)&bytesPerSector,
         (DWORD*)&numberOfFreeClusters,
         (DWORD*)&totalNumberOfClusters
       );
  
       DWORD64 div = 1000000;
	   DWORD64 unit = (bytesPerSector*sectorsPerCluster);
	   DWORD64 count = numberOfFreeClusters;
	   DWORD64 total = totalNumberOfClusters;
	   
	   
	   
	   DWORD64 freeBytes = (unit*count)/div;
	   DWORD64 totalBytes = (unit*total)/div;
	   long returnVal = freeBytes;
       return returnVal;
}


void CAbDeploymentCntrlCtrl::MakeDirectory(LPSTR path)
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

long CAbDeploymentCntrlCtrl::ValidateVersion(LPSTR localPath)
{
     CFileException exc;
     CFile fl;
	 long version = 0;
  
	 CArchive the_Archive(&fl,CArchive::load);
     BOOL res = fl.Open(localPath,CFile::modeRead, &exc);
	 if(res)
	 {
	    CString misc;
		if(the_Archive.ReadString(misc))
           version = atoi(misc);
        

     the_Archive.Close();
	 fl.Close();
	 }
     return version;
}

void CAbDeploymentCntrlCtrl::WriteVersion(LPSTR localPath, long version)
{
    CFileException exc;
    CFile fl;
	CArchive the_out_Archive(&fl,CArchive::store);
    BOOL res = fl.Open(localPath,CFile::modeCreate | CFile::modeWrite, &exc);
    the_out_Archive.WriteString( ConvToString(version) );  
	the_out_Archive.Close();
    fl.Close(); 
}

CString CAbDeploymentCntrlCtrl::ConvToString(int number)
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


int CAbDeploymentCntrlCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	windowHandle = GetSafeHwnd();
    
	::ShowWindow(windowHandle,SW_SHOW);//show window so exclusive mode is possible

	
	
    /*m_dxDeviceObj = new DxDeviceClass(2048,512,512,32);
	m_renderStateObject = new CStateManagementObj();
    int result = m_dxDeviceObj->CreateRenderingDevices(TRUE,512,512,32,windowHandle);
    if(result != 1)
	{//Failed
		AfxMessageBox("Cannot create device correctly");
		return 0;
	}//end Failed
    m_renderStateObject->InitializeAllStates(m_dxDeviceObj->m_pd3dDevice);
	//test render*/

	return 0;
}




void CAbDeploymentCntrlCtrl::OnTimer(UINT nIDEvent)
{
	if(!m_rSp)
	{
	   m_rSp = TRUE;
	   m_engineClient->CallBackTimeOpen();
	   m_engineClient->ClientCallback();
	   m_engineClient->CallBackTimeCap();
	   m_rSp = FALSE;
	}

	COleControl::OnTimer(nIDEvent);
}


//-----------------------------------------------------------//
DWORD WINAPI RenderLoopThread(LPVOID lpParameter)
{	   
	  CClientModule * classObj = (CClientModule *)lpParameter;
       
	  while(!classObj->m_initObject->m_gStatus->m_closeDownCall)
	  {
             classObj->CallBackTimeOpen();
             classObj->ClientCallback();
			 classObj->CallBackTimeCap();
	  }
return 1;
}
