#define WINVER 0x0502

#include <AfxWin.h>
#include <AfxExt.h>
#include "RMApp.h"
#include "d3d9.h"
#include "RMWin.h"
#include "CrashReporter.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

 BEGIN_MESSAGE_MAP(RMApp, CWinApp)
	//{{AFX_MSG_MAP(RMApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

DWORD WINAPI RenderCallback(LPVOID lpParameter);

RMApp::RMApp()
{
	rmwin=0;
	m_pMainWnd=0;
	m_randerCallHandle=0;
	m_workerThreadID = 0;

	
}




BOOL RMApp::InitInstance()
{
	
	m_pMainWnd->ShowWindow( SW_SHOWNORMAL );
	m_pMainWnd->UpdateWindow();
	enabled = FALSE;
    int currentMsgs = 0;
	int maxMsgsSim  = 10;

	static int testWait = 0;
	while(1)
	{	
	   rmwin->RenderLoop();
	   Sleep(0);
	   BOOL bDoingBackgroundProcessing = TRUE;
          MSG msg;
		  currentMsgs = 0;
          while( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) 
		  { 
         
            if ( !PumpMessage( ) ) 
			{ 
               bDoingBackgroundProcessing = FALSE; 
               ::PostQuitMessage(0); 
               break; 
			} 
			//if(currentMsgs > maxMsgsSim)
			   //break;
			currentMsgs++;
		  } 
		  if(!bDoingBackgroundProcessing)
			  break;

		   
	}

	//m_randerCallHandle = ::CreateThread(NULL,0,RenderCallback,rmwin,0,&m_workerThreadID);
	
	return TRUE;
}

LRESULT RMApp::ProcessWndProcException(CException* e, const MSG* pMsg)
{
	ReportMfcException(e, pMsg);
	return CWinApp::ProcessWndProcException(e, pMsg);
}

BOOL RMApp::OnIdle(LONG count) 
{ 
	//rmwin->RenderLoop();
    
	return TRUE;
}

DWORD WINAPI RenderCallback(LPVOID lpParameter)
{
  RMWin* rmwin = (RMWin*)lpParameter;
  //while(1)
     //rmwin->RenderLoop();

  return 0;
}





