// AX_EngineControlCtrl.cpp : Implementation of the CAX_EngineControlCtrl ActiveX Control class.

#include "stdafx.h"
#include "AX_EngineControl.h"
#include "AX_EngineControlCtrl.h"
#include "AX_EngineControlPropPage.h"
#include ".\ax_enginecontrolctrl.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CAX_EngineControlCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CAX_EngineControlCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CAX_EngineControlCtrl, COleControl)
	DISP_FUNCTION_ID(CAX_EngineControlCtrl, "AboutBoxX", DISPID_ABOUTBOX, AboutBoxX, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY(CAX_EngineControlCtrl, "m_port", m_port, VT_I4 )
	DISP_FUNCTION(CAX_EngineControlCtrl, "SetPort", SetPort, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CAX_EngineControlCtrl, "Startup", Startup, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CAX_EngineControlCtrl, COleControl)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CAX_EngineControlCtrl, 1)
	PROPPAGEID(CAX_EngineControlPropPage::guid)
END_PROPPAGEIDS(CAX_EngineControlCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CAX_EngineControlCtrl, "AX_ENGINECONTROL.AX_EngineControlCtrl.1",
	0x14a6e84c, 0x7f66, 0x4ba6, 0x80, 0xcd, 0x75, 0x9c, 0xe1, 0x25, 0xf2, 0xd1)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CAX_EngineControlCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID BASED_CODE IID_DAX_EngineControl =
		{ 0xCFADE9AA, 0x15CB, 0x4F5C, { 0x8D, 0x61, 0xB5, 0xD9, 0x35, 0xA3, 0xDD, 0xBF } };
const IID BASED_CODE IID_DAX_EngineControlEvents =
		{ 0x5C5A3A1, 0xB800, 0x4747, { 0x9B, 0x65, 0x80, 0xD0, 0x51, 0x68, 0x5, 0xF6 } };



// Control type information

static const DWORD BASED_CODE _dwAX_EngineControlOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CAX_EngineControlCtrl, IDS_AX_ENGINECONTROL, _dwAX_EngineControlOleMisc)



// CAX_EngineControlCtrl::CAX_EngineControlCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CAX_EngineControlCtrl

BOOL CAX_EngineControlCtrl::CAX_EngineControlCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_AX_ENGINECONTROL,
			IDB_AX_ENGINECONTROL,
			afxRegApartmentThreading,
			_dwAX_EngineControlOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


DWORD WINAPI RenderSpinner(LPVOID lpParameter);
DWORD WINAPI AXML_Parser(LPVOID lpParameter);
// CAX_EngineControlCtrl::CAX_EngineControlCtrl - Constructor

CAX_EngineControlCtrl::CAX_EngineControlCtrl()
{
	InitializeIIDs(&IID_DAX_EngineControl, &IID_DAX_EngineControlEvents);
	// TODO: Initialize your control's instance data here.
	m_port = 1050;
	m_autoStart = FALSE;
	m_engineClient = NULL;
	m_renderThreadID = NULL;
	m_renderThreadRuinning = FALSE;
    m_renderThread = NULL;
	m_closeDown = FALSE;
	m_maxTextureSize = 2048;
	m_shadowCount = 10;
	m_xmlInterfaceServer = NULL;

	m_xmlThread         = NULL;
	m_xmlThreadID       = 0;
	m_xmlThreadRuinning = FALSE;
	m_axLogger.m_logFileName = "developerTools.txt";
	m_axLogger.WriteDebugFile("Init-Dev tools",1,1);
	InitializeCriticalSection(&m_updateCriticalSection);
}



// CAX_EngineControlCtrl::~CAX_EngineControlCtrl - Destructor

CAX_EngineControlCtrl::~CAX_EngineControlCtrl()
{
    EnterCriticalSection(&m_updateCriticalSection);
	m_closeDown = TRUE;
	LeaveCriticalSection(&m_updateCriticalSection);
    Sleep(2000);


	if(m_engineClient)
	   m_engineClient->m_initObject->m_gStatus->SetCloseDownCallState(TRUE);


	Sleep(2000);


	if(m_engineClient)
	   delete m_engineClient;
	m_engineClient=0;

	DeleteCriticalSection(&m_updateCriticalSection);
}



// CAX_EngineControlCtrl::OnDraw - Drawing function

void CAX_EngineControlCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	//pdc->Ellipse(rcBounds);
}



// CAX_EngineControlCtrl::DoPropExchange - Persistence support

void CAX_EngineControlCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	short tempShort = 0;
	BOOL tempBool = 0;
    CString temp = "";
    if(PX_String(pPX,_T("m_port"),temp ))
	{
	    
       //AfxMessageBox("PORT:"+temp,0,0);
	}


	/*if(PX_Short(pPX, _T("m_port"), tempShort))
	{
		if(tempShort > 0)
		{
		   m_port = tempShort;
		   
		}
		AfxMessageBox("PORT:"+m_cablib.ConvToString((int)tempShort),0,0);
		
	}
	if(PX_Bool(pPX, _T("m_autoStart"), tempBool))
	{
		if(tempBool > 0)
		{
		   m_autoStart = tempBool;
		   
		}
		AfxMessageBox("autoStart:"+m_cablib.ConvToString((int)tempBool),0,0);
	}*/
}

void CAX_EngineControlCtrl::SetPort(int port)
{
     m_port = port;
}


// CAX_EngineControlCtrl::OnResetState - Reset control to default state

void CAX_EngineControlCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CAX_EngineControlCtrl::AboutBox - Display an "About" box to the user

void CAX_EngineControlCtrl::AboutBoxX()
{
	CDialog dlgAbout(IDD_ABOUTBOX_AX_ENGINECONTROL);
	dlgAbout.DoModal();
}

//-----------------------------------------------------------//
DWORD WINAPI RenderSpinner(LPVOID lpParameter)
{	   
	    CAX_EngineControlCtrl * classObj = (CAX_EngineControlCtrl *)lpParameter; 
		if(FAILED(CoInitialize(NULL)))
		{
           return 0;
		}

        

		while(1)
		{
		   
           EnterCriticalSection(&classObj->m_updateCriticalSection);
	       if(classObj->m_closeDown == TRUE)
	       {
		      classObj->m_renderThreadRuinning = FALSE;
              LeaveCriticalSection(&classObj->m_updateCriticalSection);
              break;   
	       }
	       LeaveCriticalSection(&classObj->m_updateCriticalSection);

           classObj->m_engineClient->m_initObject->m_userControls->MouseManagerCallback();

           classObj->m_engineClient->CallBackTimeOpen();
		   classObj->m_engineClient->ClientCallback();
		   classObj->m_engineClient->CallBackTimeCap();

		   

		   if(classObj->m_engineClient->m_initObject->m_gStatus->GetCloseDownCallState())
	          ::PostMessage(classObj->m_engineClient->m_initObject->m_gStatus->m_dxRenderer->m_topWindowHandle, WM_CLOSE, 0, 0 );
		}
		return 0;
}


// CAX_EngineControlCtrl message handlers

BOOL CAX_EngineControlCtrl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	return COleControl::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

int CAX_EngineControlCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	//fire up xml socket server
	if(m_autoStart)
	{
		Startup(m_port);
	   //m_xmlInterfaceServer = new CAX_XMLConnector(m_port);
	   //m_xmlThread = CreateThread(NULL,0,AXML_Parser,this,0,&m_xmlThreadID);//call validate/dowloader
	  // m_xmlThreadRuinning = TRUE;
	}

	return 0;
}

void CAX_EngineControlCtrl::Startup(int port)
{
	//AfxMessageBox("Startup",0,0);
    m_port = port;
	m_xmlInterfaceServer = new CAX_XMLConnector(m_port);
	m_xmlThread = CreateThread(NULL,0,AXML_Parser,this,0,&m_xmlThreadID);//call validate/dowloader
	m_xmlThreadRuinning = TRUE;
}

void CAX_EngineControlCtrl::FireEngineUp()
{
	RECT m_windowSz;
	HWND windowHandle = GetSafeHwnd();
	::GetWindowRect(windowHandle,&m_windowSz);    
	m_engineClient = new CClientModule(windowHandle,0,0,(m_windowSz.right-m_windowSz.left),(m_windowSz.bottom-m_windowSz.top),32,FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,m_maxTextureSize,m_shadowCount);
}

void CAX_EngineControlCtrl::BeginRendering()
{
    m_renderThread = CreateThread(NULL,0,RenderSpinner,this,0,&m_renderThreadID);//call validate/dowloader
	m_renderThreadRuinning = TRUE;
}
//-----------------------------------------------------------//
DWORD WINAPI AXML_Parser(LPVOID lpParameter)
{	   
	    CAX_EngineControlCtrl * classObj = (CAX_EngineControlCtrl *)lpParameter; 
		if(FAILED(CoInitialize(NULL)))
		{
           return 0;
		}
		classObj->FireEngineUp();//start engine
		while(1)
		{
           EnterCriticalSection(&classObj->m_updateCriticalSection);
	       if(classObj->m_closeDown == TRUE)
	       {
		      
              LeaveCriticalSection(&classObj->m_updateCriticalSection);
              break;   
	       }
	       LeaveCriticalSection(&classObj->m_updateCriticalSection);

		   
           WaitForSingleObject(classObj->m_xmlInterfaceServer->m_msgBuilder->m_dataIncommingEvent,1000);
           CAX_XMLCommandQueObj * cmdQue = classObj->m_xmlInterfaceServer->m_msgBuilder->GetNextCompletedMsg();
           if(cmdQue)
		   {//process
              classObj->XmlCommandParser(cmdQue);
		      delete cmdQue;
			  cmdQue = 0;
		   }//end process
		   ResetEvent(classObj->m_xmlInterfaceServer->m_msgBuilder->m_dataIncommingEvent);
		   Sleep(10);
		}
		return 0;
}


void CAX_EngineControlCtrl::XmlCommandParser(CAX_XMLCommandQueObj * cmdQue)
{
      //_stricmp //ignore case

	  //CATAGORIES
	  //CONSOLE
	  //LOAD - load assets/modules
	  //SETVAR - sets constants
      //
      
	  GlobalStatusVariables * glInterface = m_engineClient->m_initObject->m_gStatus;
	  CString errorReturn		= "";
      CString tag				= "";
	  CString limitTag			= "";
	  CString value				= "";
	  long cursorPos			= 0;
	  int tempValInt			= 0;
	  float tempValFloat		= 0.0f;
	  long anchorPos			= 0;
	  CString returnResponse    = "";

	  while(GetNextOpenTag(cmdQue->m_dataBlock, cursorPos, cmdQue->m_msgLength, tag, value))
	  {
		  errorReturn = "";
          if(tag.CompareNoCase("CONSOLE")==0)
		  {
               if(value.CompareNoCase("MIMIC")==0)
				  m_xmlInterfaceServer->SendMessage((BYTE*)cmdQue->m_dataBlock,cmdQue->m_msgLength,false);
		  }
		  else if(tag.CompareNoCase("REQUEST")==0)
		  {
			   returnResponse = "";
               if(value.CompareNoCase("WORLD OBJECTS")==0)
			   {
                  for(POSITION posLoc = glInterface->m_worldGroupList->GetHeadPosition();posLoc != NULL;)
				  {//group database
                      CWldGroupObj * partPtr = (CWldGroupObj *)glInterface->m_worldGroupList->GetNext(posLoc);
                      //build group header
                      returnResponse += "<NEW WORLD GROUP><NAME>"+partPtr->List_groupName+"</NAME><ID>"+glInterface->m_cabLib.ConvToString(partPtr->List_groupInt)+"</ID></NEW WORLD GROUP>";

                      for(POSITION wldObjPos = glInterface->m_wldObjList->GetHeadPosition();wldObjPos != NULL;)
					  {
                          CWldObject * wObj = (CWldObject *)glInterface->m_wldObjList->GetNext(wldObjPos);
					      if(wObj->List_groupNumber == partPtr->List_groupInt)
						  {//build object tag
                             returnResponse += "<ADD WORLD OBJECT>";
							 returnResponse += "<NAME>"+wObj->m_fileNameRef+"</NAME>";
							 returnResponse += "<TRC>"+glInterface->m_cabLib.ConvToString(wObj->List_identity)+"</TRC>";
							 if(wObj->m_lightObject)
							    returnResponse += "<LIGHT>YES</LIGHT>";
							 else
							    returnResponse += "<LIGHT>NO</LIGHT>";
							 if(wObj->List_triggerList->GetCount() > 0)
							    returnResponse += "<TRIGGER>YES</TRIGGER>";
							 else
							    returnResponse += "<TRIGGER>NO</TRIGGER>";
						     returnResponse += "</ADD WORLD OBJECT>";
						  }//end build object tag
					  }
				  }//end group database
				  //send info
				  m_xmlInterfaceServer->SendMessage((BYTE*)returnResponse.GetBuffer(),returnResponse.GetLength(),false);
			   }
		  }
		  else if(tag.CompareNoCase("LOAD")==0)
		  {
               GetNextOpenTag(cmdQue->m_dataBlock, cursorPos, cmdQue->m_msgLength, tag, value);
			   if(tag.CompareNoCase("FILE")==0)
			   {
			      if(glInterface->LoadByFileName(value))
				  {
					  m_axLogger.WriteDebugFile("LoadByFileName():",0,0);
					  m_axLogger.WriteDebugFile(value.GetBuffer(),0,1);
				  }
				  else
				  {
                      m_axLogger.WriteDebugFile("FAILED LoadByFileName():",0,0);
					  m_axLogger.WriteDebugFile(value.GetBuffer(),0,1);
				  }
			   }
			   else if(tag.CompareNoCase("RENDERER")==0)
			   {
                  //value = fps
			      BeginRendering();
			   }
		  }
		  else if(tag.CompareNoCase("SETVAR")==0)
		  {
			   GetNextOpenTag(cmdQue->m_dataBlock, cursorPos, cmdQue->m_msgLength, tag, value);

               if(tag.CompareNoCase("WORKING_DIRECTORY")==0)
			   {
			      //validate directory Exists and or create it
                  m_abTool.MakeDirectory(value.GetBuffer());
				  CString cachDir = value + "CACHE\\";
				  m_abTool.MakeDirectory(cachDir.GetBuffer());
			      //Set Values
			      SetCurrentDirectory(value);
                  m_engineClient->m_initObject->m_gStatus->m_installPath = value;
		          m_engineClient->m_initObject->m_gStatus->m_cacheDirectory = cachDir;
				  m_engineClient->m_initObject->m_gStatus->GL_pathMapModels = value+"\\MapsModels\\";
	              m_engineClient->m_initObject->m_gStatus->GL_pathGameFiles = value+"\\GameFiles\\";
			   }
               else if(tag.CompareNoCase("GAME_MOUSE")==0)
			   {
			      if(value.CompareNoCase("ENABLE")==0)
				  {
                     m_engineClient->m_initObject->m_gStatus->m_dxRenderer->EnableManagerCallback();
					 m_axLogger.WriteDebugFile("Enable Mouse System",0,1);
				  }
			      else if(value.CompareNoCase("ON")==0)
				  {
				     m_engineClient->m_initObject->m_gStatus->m_dxRenderer->DisableMouse();
					 m_axLogger.WriteDebugFile("Enable Game Mouse",0,1);
				  }
				  else
				  {
                     m_engineClient->m_initObject->m_gStatus->m_dxRenderer->EnableMouse();
					 m_axLogger.WriteDebugFile("Disable Game Mouse",0,1);
				  }
			   }
		  }
		  else if(tag.CompareNoCase("SPAWN")==0)
		  {
			  GetNextOpenTag(cmdQue->m_dataBlock, cursorPos, cmdQue->m_msgLength, tag, value);
               if(tag.CompareNoCase("ENTITY")==0)
			   {
				   limitTag = "/ENTITY_MAIN";
				   int entityIndex, entityType, animationOvrd;
				   D3DMATRIX matDef = glInterface->m_matrixInit2;
				   anchorPos = cursorPos;
				   animationOvrd = -1;//default is none (optional data)
                   if(!GetValueByTagFromCursor(cmdQue->m_dataBlock, anchorPos, cmdQue->m_msgLength,"INDEX", entityIndex,limitTag))
				   {
                       errorReturn = "<ERROR>ENTITY-INDEX not found</ERROR>";
					   m_xmlInterfaceServer->SendMessage((BYTE*)errorReturn.GetBuffer(),errorReturn.GetLength(),false);
					   continue;
				   }
				   if(!GetValueByTagFromCursor(cmdQue->m_dataBlock, anchorPos, cmdQue->m_msgLength,"TYPE", entityType,limitTag))
				   {
                       errorReturn = "<ERROR>ENTITY-TYPE not found 0=main 5=AI Standard 15-static 35=interactive</ERROR>";
					   m_xmlInterfaceServer->SendMessage((BYTE*)errorReturn.GetBuffer(),errorReturn.GetLength(),false);
					   continue;
				   }
				   if(!GetValueByTagFromCursor(cmdQue->m_dataBlock, anchorPos, cmdQue->m_msgLength,"POSX", matDef._41,limitTag))
				   {
                       errorReturn = "<ERROR>ENTITY-POSX not found</ERROR>";
					   m_xmlInterfaceServer->SendMessage((BYTE*)errorReturn.GetBuffer(),errorReturn.GetLength(),false);
					   continue;
				   }
				   if(!GetValueByTagFromCursor(cmdQue->m_dataBlock, anchorPos, cmdQue->m_msgLength,"POSY", matDef._42,limitTag))
				   {
                       errorReturn = "<ERROR>ENTITY-POSY not found</ERROR>";
					   m_xmlInterfaceServer->SendMessage((BYTE*)errorReturn.GetBuffer(),errorReturn.GetLength(),false);
					   continue;
				   }
				   if(!GetValueByTagFromCursor(cmdQue->m_dataBlock, anchorPos, cmdQue->m_msgLength,"POSZ", matDef._43,limitTag))
				   {
                       errorReturn = "<ERROR>ENTITY-POSZ not found</ERROR>";
					   m_xmlInterfaceServer->SendMessage((BYTE*)errorReturn.GetBuffer(),errorReturn.GetLength(),false);
					   continue;
				   }
				   GetValueByTagFromCursor(cmdQue->m_dataBlock, anchorPos, cmdQue->m_msgLength,"FORCE ANIMATION", animationOvrd,limitTag);//optional in available
				  


				   BOOL result = glInterface->ActivateMovementEntity(   NULL,
																		0,
																		matDef,
																		0,
																		0,
																		entityIndex,
																		entityType,//control type-0=main controller
																		0,
																		0,
																		FALSE,
																		TRUE,
																		FALSE,
																		-1,
																		-1,
																		NULL,
																		NULL,
																		0,//phy index ovrd(unused it seems)
																		animationOvrd,//animation override
																		glInterface->m_collisionBase,
																		glInterface->m_collisionDatabase,
																		glInterface->m_entObjRuntime,
																		glInterface->m_currentWorkingFile);

				   if(!result)
				   {//spawn failed
                      errorReturn = "<ERROR>ENTITY_MAIN-Function Return Failed</ERROR>";
					  m_xmlInterfaceServer->SendMessage((BYTE*)errorReturn.GetBuffer(),errorReturn.GetLength(),false);
				   }//end spawn failed
			   }
			   
		  }
	  }

}

bool CAX_EngineControlCtrl::GetNextOpenTag(BYTE * data, long & currentPosition, long & length, CString & tag, CString & value)
{
    int stage = 0;
	tag = "";
	value = "";
    while(currentPosition < length)
	{
        switch(stage)
		{
		   case 0:
              while(data[currentPosition]!='<' && currentPosition+1 < length)
			  {

                    currentPosition++;
			  }
              //stage = 1;
			  if(currentPosition+1 < length)
              if(data[currentPosition] == '<')
			     if(data[currentPosition+1] != '/')
			        stage = 1;
		   break;
		   case 1:
              while(data[currentPosition]!='>' && currentPosition < length)
			  {
				    tag += (char)data[currentPosition];
                    currentPosition++;
			  }
			  stage = 2;
		   break;
		   case 2://get value
              while(data[currentPosition]!='<' && currentPosition < length)
			  {
				    value += (char)data[currentPosition];
                    currentPosition++;
			  }
			
			  if(data[currentPosition]=='<')
                 return true;
		   break;
		}
		currentPosition++;
	}
	return false;
}

bool CAX_EngineControlCtrl::GetValueByTagFromCursor(BYTE * data, long & currentPosition, long & length,CString inputTag, float & value, CString limitTag)
{
     CString tempVal = "";
     bool res = GetValueByTagFromCursor(data,currentPosition,length,inputTag, tempVal,limitTag);
     if(!res)
         return res; 

     value = (float)atof(tempVal);

	 return res; 
}

bool CAX_EngineControlCtrl::GetValueByTagFromCursor(BYTE * data, long & currentPosition, long & length,CString inputTag, int & value, CString limitTag)
{
	 CString tempVal = "";
     bool res = GetValueByTagFromCursor(data,currentPosition,length,inputTag, tempVal,limitTag);
     if(!res)
         return res; 

     value = atoi(tempVal);

	 return res; 
}

bool CAX_EngineControlCtrl::GetValueByTagFromCursor(BYTE * data, long & currentPosition, long & length,CString inputTag, CString & value, CString limitTag)
{
    int stage = 0;
	CString localTag = "";
	value = "";
    while(currentPosition < length)
	{
        switch(stage)
		{
		   case 0:
              while(data[currentPosition]!='<' && currentPosition+1 < length)
			  {

                    currentPosition++;
			  }
              //stage = 1;
			  if(currentPosition+1 < length)
              if(data[currentPosition] == '<')
			     if(data[currentPosition+1] != '/')
				 {
			        stage = 1;
				 }
				 else if(limitTag.GetLength() > 0)
				 {//check if limit tag
                    localTag = "";
					currentPosition++;//skip /
                    while(data[currentPosition]!='>' && currentPosition < length)
			        {
				       localTag += (char)data[currentPosition];
                       currentPosition++;
			        }
					if(localTag.CompareNoCase(limitTag)==0)
					   return false;
				 }//end check if limit tag
		   break;
		   case 1:
		      localTag = "";
              while(data[currentPosition]!='>' && currentPosition < length)
			  {
				    localTag += (char)data[currentPosition];
                    currentPosition++;
			  }
              if(localTag.CompareNoCase(inputTag)!=0)
			  {//no match
				  stage = 0;//continue to next tag
				  continue;
			  }//no match
			  stage = 2;
		   break;
		   case 2://get value
              while(data[currentPosition]!='<' && currentPosition < length)
			  {
				    value += (char)data[currentPosition];
                    currentPosition++;
			  }
			
			  if(data[currentPosition]=='<')
                 return true;
		   break;
		}
		currentPosition++;
	}



	return false;
}

bool CAX_EngineControlCtrl::SeekPositionToNextValue(BYTE * data, long & currentPosition, long & length,CString & nextValue)
{




	return false;
}
