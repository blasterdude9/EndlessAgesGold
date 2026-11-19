// ClientSimulatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ClientSimulator.h"
#include "ClientSimulatorDlg.h"

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


// CClientSimulatorDlg dialog




CClientSimulatorDlg::CClientSimulatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientSimulatorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientSimulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_serverIP, m_serverIP);
	DDX_Control(pDX, IDC_EDIT_Username, m_username);
	DDX_Control(pDX, IDC_EDIT_password, m_password);
	DDX_Control(pDX, IDC_CHECK_EnableSingleLogonTest, m_singleLogonModeChk);
	DDX_Control(pDX, IDC_LIST_ClientList, m_clientConnectionsList);
	DDX_Control(pDX, IDC_LIST_logOutput, m_logOutput);
	DDX_Control(pDX, IDC_EDIT_sequenceStart, m_sequenceStart);
	DDX_Control(pDX, IDC_EDIT_sequenceEnd, m_sequenceEnd);
	DDX_Control(pDX, IDC_EDIT_radius, m_radius);
}

BEGIN_MESSAGE_MAP(CClientSimulatorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LogonStressTest, &CClientSimulatorDlg::OnBnClickedButtonLogonstresstest)
	ON_BN_CLICKED(IDC_BUTTON_GamePlayStress, &CClientSimulatorDlg::OnBnClickedButtonGameplaystress)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_AddManualAccount, &CClientSimulatorDlg::OnBnClickedButtonAddmanualaccount)
	ON_BN_CLICKED(IDC_BUTTON_Refresh, &CClientSimulatorDlg::OnBnClickedButtonRefresh)
	ON_WM_TIMER()
END_MESSAGE_MAP()


//thread function def
DWORD WINAPI ClientManager(LPVOID lpParameter);
// CClientSimulatorDlg message handlers

BOOL CClientSimulatorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//init random chat
	m_randomChat[0] = "marco";
	m_randomChat[1] = "polo";
	m_randomChat[2] = "wth are you guys talking about???";
	m_randomChat[3] = "extra long text string *&&^@%$*%$!)(!@ yes no maybe so today yesterday tommorow maybe? i dont know, maybe ill go. well well well whos here? what zone are we in, why are we here?";//overun attemps
	m_randomChat[4] = "what do you mean calm down, underpod arena 1VS1 now!!! ";
	m_randomChat[5] = "";//possible crash attempt
	m_randomChat[6] = "whats this about cows? possibly a rare drop?";
	m_randomChat[7] = "this is crazy talk";
	m_randomChat[8] = "marco marco";
	m_randomChat[9] = "polo polo";
	//end init random chat

	ReadConfigFile("sim.cfg");

	m_threadHandleCount = 0;
	m_currentThreadCreate = 0;

    ZeroMemory(m_accounts,sizeof(acStruct)*5000);
    m_accountAddTracker = 0;

	m_logBothTextFileAndList = TRUE;
    //thread handle bank
	m_threadHandles = NULL;
	m_threadIDs = NULL;
	m_threadRunningCount = 0;

    ClearLog();
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
	InitializeCriticalSection(&m_mSect);
	InitializeCriticalSection(&m_logSect);
	
	m_closeDown = FALSE;

    

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientSimulatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientSimulatorDlg::OnPaint()
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
HCURSOR CClientSimulatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CClientSimulatorDlg::OnBnClickedButtonLogonstresstest()
{
	// TODO: Add your control notification handler code here
}

void CClientSimulatorDlg::OnBnClickedButtonGameplaystress()
{
	
   
	//check for sequence 
	CString temp,temp2,radius,username,password,finalUsername,tempIndex;
	m_sequenceStart.GetWindowTextA(temp);
	m_sequenceEnd.GetWindowTextA(temp2);
	if(temp.GetLength() > 0)
	{//valid sequence feild
       int sStart = atoi(temp);
       int sEnd = atoi(temp2);
	   m_username.GetWindowTextA(username);
       m_password.GetWindowTextA(password);
	   m_radius.GetWindowTextA(radius);
	   float radiusfloat = (float)atof(radius);
       int loopCount = sEnd - sStart;
       for(int aLoop=sStart;aLoop<sEnd;aLoop++)
	   {//add account loop
		   tempIndex.Format("%i",aLoop);
           finalUsername = username + tempIndex;
		   
           CopyMemory(m_accounts[m_accountAddTracker].username,finalUsername.GetBuffer(),finalUsername.GetLength());
           CopyMemory(m_accounts[m_accountAddTracker].password,password.GetBuffer(),password.GetLength());

		   //setup random position
           m_accounts[m_accountAddTracker].position = GetRandomPositionInRadius(radiusfloat,m_accounts[m_accountAddTracker].position,.01f);

	       m_accountAddTracker++;
	   }//end add account loop
	   InListAccounts(m_accountAddTracker-1);
	}//end valid sequence feild

	m_threadHandleCount = m_accountAddTracker;
    if(m_threadHandleCount == 0)
	   m_threadHandleCount = 1;



    m_threadHandles = new HANDLE[m_threadHandleCount];
    m_threadIDs     = new DWORD[m_threadHandleCount];

	m_currentThreadCreate = 0;

	SetTimer(1,1000,0);
}

void CClientSimulatorDlg::OnDestroy()
{
	CDialog::OnDestroy();

	EnterCriticalSection(&m_mSect);
	m_closeDown = TRUE;
	while(m_threadRunningCount > 0)
	{
		  LeaveCriticalSection(&m_mSect);
          //Log("Waiting for threads to close down");
          Sleep(500);
		  EnterCriticalSection(&m_mSect);
	}
	LeaveCriticalSection(&m_mSect);

    for(int loop=0;loop<m_threadHandleCount;loop++)
	{
        CloseHandle(m_threadHandles[loop]);
	}
    if(m_threadHandles)
	   delete [] m_threadHandles;
	m_threadHandles = 0;
	if(m_threadIDs)
	   delete [] m_threadIDs;
	m_threadIDs = 0;
    
	WriteConfigFile("sim.cfg");
	Log("Exit Clean");
	DeleteCriticalSection(&m_mSect);
	DeleteCriticalSection(&m_logSect);
	
}

void CClientSimulatorDlg::OnBnClickedButtonAddmanualaccount()
{ 
    CString temp;	
	m_username.GetWindowTextA(temp);
    CopyMemory(m_accounts[m_accountAddTracker].username,temp.GetBuffer(),temp.GetLength());
	m_password.GetWindowTextA(temp);
    CopyMemory(m_accounts[m_accountAddTracker].password,temp.GetBuffer(),temp.GetLength());

	m_accountAddTracker++;
	InListAccounts(m_accountAddTracker-1);
}

void CClientSimulatorDlg::InListAccounts(int sel)
{
    m_clientConnectionsList.ResetContent();
	CString packetsSent,packetsRecv;

	for(int loop=0;loop < m_accountAddTracker;loop++)
	{
		EnterCriticalSection(&m_mSect);

		packetsSent.Format(" Packets Sent:%i",m_accounts[loop].packetsSent);
		packetsRecv.Format(" Packets Recv:%i",m_accounts[loop].packetsReceived);

        m_clientConnectionsList.AddString(m_accounts[loop].username + packetsSent + packetsRecv);

		LeaveCriticalSection(&m_mSect);
	}
    if(sel > -1)
	   m_clientConnectionsList.SetCurSel(sel);
}

void CClientSimulatorDlg::ClearLog()
{
     m_realtimeLogger.m_logFileName  = "simulationR.txt";
	 m_realtimeLogger.WriteDebugFile("initialized",1,1);
}

void CClientSimulatorDlg::Log(CString msg)
{
	 EnterCriticalSection(&m_logSect);
     if(m_logBothTextFileAndList)
	    m_realtimeLogger.WriteDebugFile(msg.GetBuffer(),0,1);
	 m_logOutput.AddString(msg);
	 LeaveCriticalSection(&m_logSect);
}


void CClientSimulatorDlg::OnBnClickedButtonRefresh()
{
	int selection = m_clientConnectionsList.GetCurSel();
	InListAccounts(selection);
}

D3DVECTOR CClientSimulatorDlg::GetRandomPositionInRadius(float radius,D3DVECTOR origin,float minimum)
{
      D3DVECTOR locationBuild;
	  locationBuild.x = 0.0f;
	  locationBuild.y = 0.0f;
	  locationBuild.z = 0.0f;
	  float matrixDistloc[4][4];
	  float matrixOriginloc[4][4];
	  float matrixRatationloc[4][4];
      float RadiusDist = (float)( rand()%((int)radius) );
	  if(RadiusDist < minimum)
		 RadiusDist = minimum;
      float RotationDist =(float)( rand()%360 );
	  GL_matrixFunctions.Translate ( origin.x,origin.y,origin.z,matrixOriginloc );//origin of roam area
	  GL_matrixFunctions.Translate ( 0.0f,0.0f,RadiusDist,matrixDistloc );//z dist
      GL_matrixFunctions.Rotate ( 0.0f,RotationDist,0.0f,matrixRatationloc );//yaw rotation
      //end get new location to travel to in radius
      //move point
	  locationBuild = GL_matrixFunctions.UpdatePolygonVertex(matrixDistloc ,locationBuild);//translation
      locationBuild = GL_matrixFunctions.UpdatePolygonVertex(matrixRatationloc ,locationBuild);//rotation
	  locationBuild = GL_matrixFunctions.UpdatePolygonVertex(matrixOriginloc ,locationBuild);//offset from origin
      return locationBuild;
}

//--------------------------------------------------------------------------//
//----------Per logon client thread-----------------------------------------//
//--------------------------------------------------------------------------//
DWORD WINAPI ClientManager(LPVOID lpParameter)
{	   
	    CClientSimulatorDlg * classObj = (CClientSimulatorDlg *)lpParameter; 
		if(FAILED(CoInitialize(NULL)))
		{
           return 0;
		}
		EnterCriticalSection(&classObj->m_mSect);
		int thisThreadIndex = classObj->m_threadRunningCount++;
		LeaveCriticalSection(&classObj->m_mSect);
        //prep vars to be used moving into processor loop
		classObj->Log("Client thread started up");
		DWORD closeStateDuration = 1000;
		DWORD closeStateStamp = GetTickCount();
        HWND windowHandle = classObj->GetSafeHwnd();
        CClientModule * engineClient = new CClientModule(windowHandle,0,0,0,0,0,FALSE,FALSE,FALSE,FALSE,TRUE,TRUE,0,0,TRUE);//makes a non-graphices client module

		classObj->Log("Client object initialized");
		EnterCriticalSection(&classObj->m_mSect);
	    classObj->m_serverIP.GetWindowTextA(engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_IPaddress);
		engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_currentVersion = 252;//local version
		if(classObj->m_singleLogonModeChk.GetCheck())
		{//get username and pass from the single logon interface
           classObj->m_username.GetWindowTextA(engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_userName);
		   classObj->m_password.GetWindowTextA(engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_passWord);
		}//end get username and pass from the single logon interface
		else
		{//get from list
		   engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_userName = classObj->m_accounts[thisThreadIndex].username;
		   engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_passWord = classObj->m_accounts[thisThreadIndex].password;
		}

		D3DVECTOR position,dirVect,upVect;
		ZeroMemory(&position,sizeof(D3DVECTOR));
		ZeroMemory(&dirVect,sizeof(D3DVECTOR));
		ZeroMemory(&upVect,sizeof(D3DVECTOR));
		position = classObj->m_accounts[thisThreadIndex].position;
		
		
        LeaveCriticalSection(&classObj->m_mSect);
        //start connection sequence
        engineClient->m_initObject->m_gStatus->m_multiplayerObject->EnableClient(engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_IPaddress,engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_currentVersion);
		int connectTryCount = 30;
		int trace = 0;
		while(!engineClient->m_initObject->m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
        {//wait till connection establishes before selecting character
           Sleep(1000);
           if(trace == 30)
              break;
		   trace++;
		}
		if(!engineClient->m_initObject->m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		{//could not connect abort
            if(engineClient)
			{
			   delete engineClient;
			   engineClient = 0;
			}
			classObj->Log("**Connection to game server could not be established");
			CoUninitialize();
            return 0;
		}//end could not connect abort
		classObj->Log("Connection to game server established:"+engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_userName);
        //play first character if available if not we are gunna have problems
		classObj->Log("Auto selecting character index 0");
		int characterIndex = 0;
		

        DWORD sendStamp = GetTickCount();

		
		upVect.y = 1.0f;
		dirVect.z = -1.0f;

		LPVOID lpMessage = NULL;
        DPNID messageFrom = 0;
        int sizeTracker = 0;

		int entityDBIndex = 0;
		int networkAssignedID = 0;

		BOOL startCommunications = FALSE;
        engineClient->m_initObject->m_clientNet->MultiplayerDelayComm = 250;
		classObj->Log("Wait for spawn message");
		while(1)
		{//primary processor loop
		   
		   if(GetTickCount()-closeStateStamp > closeStateDuration)
		   {//possible exit
			   EnterCriticalSection(&classObj->m_mSect);
			   if(classObj->m_closeDown == TRUE)
			   {
				  LeaveCriticalSection(&classObj->m_mSect);
				  break;   
			   }
			   LeaveCriticalSection(&classObj->m_mSect);
			   closeStateStamp = GetTickCount();
		   }//end possible exit
       
           

		   //periodic send
           //if(GetTickCount()-sendStamp > engineClient->m_initObject->m_clientNet->MultiplayerDelayComm)
		   //{
               while(engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_packetQue->GetNextPacket_safe(&sizeTracker,(BYTE**)&lpMessage,&messageFrom)==TRUE)
		       {//if theres a message handle it
			      //look for spawn packet to start trigering sends

                  BYTE * byteBlockData = (BYTE*)lpMessage;
                  if(sizeTracker < 1)
				     continue;
  
				  EnterCriticalSection(&classObj->m_mSect);
				  classObj->m_accounts[thisThreadIndex].packetsReceived++;

			      LeaveCriticalSection(&classObj->m_mSect);
  
				  if(byteBlockData[0] == 2)
				  {//reconnect sim
                     LPRECONNECT_STRUCT formatedMessage = (LPRECONNECT_STRUCT)lpMessage;
		
		             engineClient->m_initObject->m_gStatus->ConnectToServerNode(formatedMessage->ipaddress);
				  }//end reconnect sim


				  if(byteBlockData[0] == 19)
				  {
                     engineClient->m_initObject->m_clientNet->m_pingInQuerry = FALSE;
                     engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_lastMovMsgRecievedTime = GetTickCount();
				  }

                  if(byteBlockData[0] == 8)
				  {
                     LPSPAWN_STRUCT formatedMessage = (LPSPAWN_STRUCT)lpMessage;
                     entityDBIndex = formatedMessage->dbIndex;
						
                     engineClient->m_initObject->m_gStatus->m_multiplayerObject->SendSpawnMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH,
                                                                                              NULL,
								                                                              entityDBIndex,//entityRuntimeObj->List_refModel,
								                                                              formatedMessage->worldName,//worldName,
										                                                      1,//entityRuntimeObj->List_traceNumber,
										                                                      -1,//useNewPresets
										                                                      characterIndex,//m_multiplayerObject->m_currentCharacterInUse,
										                                                      -1);//aiCfgIndex);
				     
                     classObj->Log("sending spawn msg");
				  }

				  if(byteBlockData[0] == 38)
				  {//guarenteed block containing network id
				     startCommunications = TRUE;//communications can begin

                       BYTE * blockCast = (BYTE*)lpMessage;
					   LPENTITYGUPDATE_HEADER formatedMessage = (LPENTITYGUPDATE_HEADER)lpMessage;
					   int dataTracer = sizeof(ENTITYGUPDATE_HEADER);

					   if(formatedMessage->m_attributeMsgCount > 0)
					   {//attribute batch coming in
						  int byteSize = sizeof(ATTRIB_STRUCT)*formatedMessage->m_attributeMsgCount;
						  ATTRIB_STRUCT * attribArray = new ATTRIB_STRUCT[formatedMessage->m_attributeMsgCount];
						  BYTE * atCast = (BYTE*)attribArray;
						  
						  for(int i=0;i<byteSize;i++)
						  {
							  atCast[i] = blockCast[dataTracer];
							  dataTracer++;
						  }
						  for(int atLoop=0;atLoop<formatedMessage->m_attributeMsgCount;atLoop++)
						  {
						      if(attribArray[atLoop].attribute == 8)
							  {//get communication link
                                 networkAssignedID = attribArray[atLoop].uniInt;
                                 classObj->Log("assigned net id");
							  }//end get communication link
						  }
						  delete [] attribArray;
						  attribArray = 0;
					    }//end attribute batch coming in
                     //classObj->Log("start send/recv");
				  }

				  if(byteBlockData[0] == 3)
				  {//normally would call character select list , then select the char to play
                     classObj->Log("Requesting to play character");
					 engineClient->m_initObject->m_gStatus->m_netGameLogicObject->SendAttribMsg(     DPNSEND_GUARANTEED,
														                                DPID_SERVERPLAYER,
																						9,
																						characterIndex,
																						0,
																						engineClient->m_initObject->m_gStatus->m_localContentClientVersion,
																						14,//VERSION EXPLOIT
																						NULL);

	                 engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_currentCharacterInUse = characterIndex;
					 engineClient->m_initObject->m_clientNet->SendGuerenteedCycleMsgToServer();
				  }//normally would call character select list , then select the char to play

				  delete [] byteBlockData;
				  byteBlockData = 0;
		       }//end if theres a message handle it

			   if(startCommunications)
			   if(engineClient->m_initObject->m_clientNet->m_pingInQuerry == FALSE)
			   {//start processing randoms
				  srand(GetTickCount()+networkAssignedID);
                  if(rand()%1000 < SPEAKPERCENTAGE)
				  {//talk
                     int chatSel = rand()%MAXRANDOMCHATCOUNT;
                     /*engineClient->m_initObject->m_gStatus->m_netGameLogicObject->SendTextMsg(  DPNSEND_GUARANTEED,
																								DPID_SERVERPLAYER,
																								classObj->m_randomChat[chatSel],
																								networkAssignedID,
																								NULL,
																								0);*/

					 engineClient->m_initObject->m_gStatus->m_netGameLogicObject->SendTextMsg(	DPNSEND_GUARANTEED,
																								DPID_SERVERPLAYER,
																								SERVER_COMMANDS::SCMD_UNKNOWN,
																								classObj->m_randomChat[chatSel],
																								networkAssignedID,
																								NULL,
																								TEXT_MESSAGE_CATEGORIES::TMC_BROADCAST);
				  }//end talk
				  srand(GetTickCount()+networkAssignedID+11);
				  if(rand()%1000 < REQ_INVENTORY_PERCENTAGE) 
				  {//request inventory random
                     //Send Attrib Message for Inventory List
					 engineClient->m_initObject->m_gStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																									DPID_SERVERPLAYER,
																									19,
																									51,//Requested Attrib ID
																									networkAssignedID,
																									0,
																									3, //Filter Type 0 = none, 1 = skills only, 2 = spells only, 3 = no skills / spells
																									NULL);//request character inventory

				  }//end request inventory random
			      srand(GetTickCount()+networkAssignedID+25);
				  if(rand()%1000 < REQ_STORE_INVENTORY_PERCENTAGE) 
				  {//request store inventory random
						CString command = ":BUY";
						engineClient->m_initObject->m_gStatus->SendChatString(command);
				  }

				  /*m_netGameLogicObject->SendFireMsg(  DPNSEND_NOCOMPLETE,
															translateAhead,
															fireRangeIndex,
															DPID_SERVERPLAYER,
															entityPtr->List_networkDefinedID);*/

			   }//end start processing randoms

			   if(startCommunications)
			   if(engineClient->m_initObject->m_clientNet->m_pingInQuerry == FALSE || GetTickCount()-engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_lastMovMsgRecievedTime > 1000)
			   {//spawned

               LPSHRUNK_TOSVRMOV_STRUCT movStruct = engineClient->m_initObject->m_clientNet->GetMovMsg( DPNSEND_NOCOMPLETE | DPNSEND_PRIORITY_HIGH,
																										DPID_SERVERPLAYER,
																										position,
																										networkAssignedID,//mainPtr->List_networkDefinedID,
																										dirVect, //here are the dir and up vectors
																										upVect,
																										0,
																										0,
																										entityDBIndex,//userPtr->List_refModel,
																										0,//animInUseCnv,
																										0,//mainPtr->List_skeletalObject->m_secondaryAnimationInUse,
																										(long)engineClient->m_initObject->m_gStatus->GetElaspeTimeMS(engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_timeElStamp));
		        //Build data packet
		        int fireMsgs = (int)engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_fireOutBox->GetCount();
                int msgSize = (sizeof(FIRE_STRUCT)*fireMsgs)+sizeof(SHRUNK_TOSVRMOV_STRUCT)+2;
		        byte * msgBlock = new byte[msgSize];
		        int dataTrace = 0;
                msgBlock[dataTrace++] = 5;//catagorie 5
		        msgBlock[dataTrace++] = fireMsgs;//fire msg count
		        CopyMemory(&msgBlock[dataTrace],movStruct,sizeof(SHRUNK_TOSVRMOV_STRUCT));
		        dataTrace += sizeof(SHRUNK_TOSVRMOV_STRUCT);
				for(POSITION posLoc = engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_fireOutBox->GetHeadPosition();posLoc != NULL;)
				{
					 CPacketObj * packPtr = (CPacketObj *)engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_fireOutBox->GetNext(posLoc);
					 CopyMemory(&msgBlock[dataTrace],packPtr->m_data,sizeof(FIRE_STRUCT));
					 dataTrace += packPtr->m_size;
				}
				engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_fireOutBox->Flush();//Flush it
				engineClient->m_initObject->m_gStatus->m_multiplayerObject->SendData((BYTE *)  msgBlock,
																                               dataTrace,
																                               DPNSEND_NOCOMPLETE | DPNSEND_PRIORITY_HIGH,
																                               DPID_SERVERPLAYER,
																                               engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_packetTimeout);
				 delete movStruct;
				 movStruct = 0;
				 
				 delete [] msgBlock;
				 msgBlock = 0;
 
			   sendStamp = GetTickCount();
			   EnterCriticalSection(&classObj->m_mSect);
			   classObj->m_accounts[thisThreadIndex].packetsSent++;
			   LeaveCriticalSection(&classObj->m_mSect);
			   engineClient->m_initObject->m_clientNet->m_pingInQuerry = TRUE;
			   Sleep(0);
			   engineClient->m_initObject->m_clientNet->SendGuerenteedCycleMsgToServer();
		   }
           //sleep delay
		   Sleep(engineClient->m_initObject->m_clientNet->MultiplayerDelayComm);
		}//end primary processor loop
        if(engineClient)
		{
		   if(engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_eventLog)
		   {
	           engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_eventLog->SafeDelete();
	           delete engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_eventLog;
			   engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_eventLog = 0;
		   }
		   if(engineClient)
	          engineClient->m_initObject->m_gStatus->SetCloseDownCallState(TRUE);
		   Sleep(300);
		   delete engineClient;
		   engineClient = 0;
		}
		
		
        //classObj->Log("Thread Exit");
		EnterCriticalSection(&classObj->m_mSect);
		classObj->m_threadRunningCount--;
		LeaveCriticalSection(&classObj->m_mSect);
		CoUninitialize();
		return 0;
}

void CClientSimulatorDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(m_currentThreadCreate<m_threadHandleCount)
	{
        m_threadHandles[m_currentThreadCreate] = CreateThread(NULL,0,ClientManager,this,0,&m_threadIDs[m_currentThreadCreate]);
	    m_currentThreadCreate++;
	}

	CDialog::OnTimer(nIDEvent);
}

void CClientSimulatorDlg::WriteConfigFile(CString filename)
{
	CFileException exc;
    CFile fl;
	CArchive the_out_Archive(&fl,CArchive::store);
    BOOL res = fl.Open(filename,CFile::modeCreate | CFile::modeWrite, &exc);
    if(!res)
	    return;

	CString username,password,ip,radius,sStart,sEnd;
    m_serverIP.GetWindowText(ip);
	m_username.GetWindowText(username);
	m_password.GetWindowText(password);
	m_sequenceStart.GetWindowText(sStart);
	m_sequenceEnd.GetWindowText(sEnd);
	m_radius.GetWindowText(radius);


	the_out_Archive.WriteString( ip );  
	the_out_Archive.WriteString( "\r\n"); 
	the_out_Archive.WriteString( username );  
	the_out_Archive.WriteString( "\r\n"); 
	the_out_Archive.WriteString( password );  
	the_out_Archive.WriteString( "\r\n"); 
	the_out_Archive.WriteString( sStart );
	the_out_Archive.WriteString( "\r\n"); 
	the_out_Archive.WriteString( sEnd );  
	the_out_Archive.WriteString( "\r\n"); 
	the_out_Archive.WriteString( radius );
	the_out_Archive.WriteString( "\r\n"); 
 
	
	the_out_Archive.Close();
    fl.Close(); 
}

void CClientSimulatorDlg::ReadConfigFile(CString filename)
{
     CFileException exc;
     CFile fl;
  
	 CArchive the_Archive(&fl,CArchive::load);
     BOOL res = fl.Open(filename,CFile::modeRead, &exc);
	 if(res)
	 {
        CString username,password,ip,radius,sStart,sEnd;

        the_Archive.ReadString(ip);
		the_Archive.ReadString(username);
		the_Archive.ReadString(password);
		the_Archive.ReadString(sStart);
		the_Archive.ReadString(sEnd);
		the_Archive.ReadString(radius);

		m_serverIP.SetWindowText(ip);
	    m_username.SetWindowText(username);
		m_password.SetWindowText(password);
		m_sequenceStart.SetWindowText(sStart);
		m_sequenceEnd.SetWindowText(sEnd);
		m_radius.SetWindowText(radius);

        the_Archive.Close();
	    fl.Close();
	   return;
	 }
}