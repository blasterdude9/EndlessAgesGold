#define D3D_OVERLOADS
#define WINVER 0x0502

#include <AfxWin.h>
#include <AfxExt.h>
#include "stdafx.h"
#include <dinput.h>

#include <d3d9.h>
#include "RMApp.h"
//#include "RMWin.h"
#include "EngineXWin.h"
#include "resource.h"
#include <stdio.h>
#include <conio.h>
#include <direct.h>
#include <stdlib.h>
#include <process.h>
#include "getdxver.h"
#include "axencryptionlib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(RMWin, CFrameWnd)
	//{{AFX_MSG_MAP(RMWin)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

long RMWin::mousex;
long RMWin::mousey;
UINT RMWin::mousestate;

RMWin::RMWin()
{
    //HRESULT hr = AtlAxWinInit();
    //check for new patcher if found delete old and rename new then execute
    CFileException patchCheckExe;
    CFile patchCheckfl;
	 Sleep(3000);
	 BOOL res = patchCheckfl.Open("patchUpdate.exe",CFile::modeRead, &patchCheckExe);
	 if(res)
	 {
	    patchCheckfl.Close();
	    CString tempOld = "Endlessages.exe";  
		CString tempNew = "patchupdate.exe";  

	    DeleteFile("Endlessages.exe");
		Sleep(100);

		res = MoveFile(tempNew,tempOld);
		if(res)
		{//success
		   char *args[4], prog[80];
           ZeroMemory(prog,80);
           
           CopyMemory(prog,tempOld.GetBuffer(),tempOld.GetLength());
	       args[0] = prog;
           args[1] = "exec??";
           args[2] = "two";
           args[3] = NULL;
           _execv( prog,args );
		   exit(1);
		}//end success
	 }
    //end check for new patcher if found delete old and rename new then execute
	m_userControls = NULL;//new UserController(hwndLoc,m_gStatus);
	m_sceneAssembler = NULL;
	m_clientEngine = NULL;
    m_gStatus = NULL;
    m_userControls = NULL;
	m_sceneAssembler = NULL;
	m_cNetworkObject = NULL;
	m_menuManagement = NULL;
	m_rendermanagerEd = NULL;
	m_wideScreen = FALSE;
    m_windowStateGL = 0;
	
	//end load specific DLLs
    
	m_curModeHieght  = 480;
    m_curModeWidth   = 640;
    m_curModeDepth   = 16;
	m_lastModeHieght = 0;
    m_lastModeWidth  = 0;
	m_lastModeDepth  = 0;
	
	CursorIsDisplayed = TRUE;    
    #ifndef _ClientOutput 
	 CString CurDir;
     CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);

	 CFileException exc2;
     CFile fl2;
	 res = fl2.Open("highRez.aut",CFile::modeRead, &exc2);
	 if(res == FALSE){
		
	 }
	 else
	 {
		 m_wideScreen = TRUE;
	 fl2.Close();
	 }
     SetCurrentDirectory(MainDir);//
	 #endif

	m_appFullscreen = FALSE;
	m_showEditor = TRUE;
	
    CoInitialize(NULL);
	Initvars();
}

RMWin::RMWin(int w, int h)
{
    
	winrect.right=w;
	winrect.bottom=h;
}



void RMWin::Initvars()
{
	//colormodel=D3DCOLOR_RGB;
	winrect=rectDefault;
    RMApp* pApp = (RMApp*)AfxGetApp();
    pApp->rmwin = this;
}



BOOL RMWin::Create(const CString& sTitle,int icon,int menu)
{
    CString sClassName;
	BOOL EffectsLogged = FALSE;
    BOOL resultLoc = 0;
  
	winrect.top = 0;
	winrect.left = 0;
	instanceHandle = AfxGetInstanceHandle();
	sClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
			LoadCursor( 0, IDC_ARROW ),
			0, LoadIcon(instanceHandle,MAKEINTRESOURCE(icon)) );
	
	#ifdef _ClientOutput 
	   winrect.right = m_curModeWidth;
       winrect.bottom = m_curModeHieght;
	   //winrect.right = 720;
       //winrect.bottom = 480;
       resultLoc=CFrameWnd::Create( sClassName, "EXNGN", WS_POPUP, winrect, 0, 0, NULL );
    #endif
    #ifndef _ClientOutput 
      winrect.right = m_curModeWidth;
      winrect.bottom = m_curModeHieght;
	  if(m_showEditor == TRUE){//set editor rect
		 RECT editorRect;
		 editorRect.top = 0;
		 editorRect.left = 0;
		 editorRect.right = 1024;//810;
		 editorRect.bottom = 550;//450;
		 m_curModeWidth = editorRect.right;
		 m_curModeHieght = editorRect.bottom;
	     resultLoc=CFrameWnd::Create( sClassName, "Aura Sim Engine", WS_OVERLAPPEDWINDOW, editorRect, 0,MAKEINTRESOURCE(menu) ); 
	  }//end set editor rect
	  else{
	  resultLoc=CFrameWnd::Create( sClassName, "Aura Sim Engine", WS_POPUP, winrect, 0,0,WS_EX_APPWINDOW);
	  }
    #endif


   
   
	return resultLoc;
}

int RMWin::OnCreate(LPCREATESTRUCT)
{
    HRESULT r;
     ::ShowWindow(m_hWnd,SW_SHOW);//show window so exclusive mode is possible
	 
     #ifndef _ClientOutput 
	   if(m_appFullscreen == FALSE)
	   {
	    if(m_wideScreen == TRUE)
		{
           m_curModeWidth = 1024;
           m_curModeHieght = 768;
		   m_curModeDepth = 16;
		}
		else
		{
           m_curModeWidth = 640;
           m_curModeHieght = 480;
		   m_curModeDepth = 16;
		}
	   }
	   //CClientModule(windowHandle,0,0,(m_windowSz.right-m_windowSz.left),(m_windowSz.bottom-m_windowSz.top),32,FALSE,FALSE,TRUE,TRUE,TRUE,TRUE,m_maxTextureSize,m_shadowCount,FALSE)
	   m_clientEngine				= new CClientModule(m_hWnd,0,0,m_curModeWidth,m_curModeHieght,m_curModeDepth,FALSE,TRUE/*edit mode*/,FALSE,TRUE,TRUE,TRUE,2048,10,FALSE);
     #endif
     #ifdef _ClientOutput 
	   m_curModeWidth = 1024;//set default
       m_curModeHieght = 768;//set default
	   m_curModeDepth = 16;//set default
	   //::ShowCursor(FALSE);
	   m_clientEngine				= new CClientModule(m_hWnd,0,0,m_curModeWidth,m_curModeHieght,m_curModeDepth,TRUE,FALSE,FALSE,TRUE,TRUE,TRUE,2048,10,FALSE);
	   //m_clientEngine				= new CClientModule(m_hWnd,0,0,m_curModeWidth,m_curModeHieght,m_curModeDepth,FALSE,FALSE,FALSE,TRUE,TRUE,TRUE);
	   /*m_curModeWidth = 720;//set default
       m_curModeHieght = 480;//set default
	   m_curModeDepth = 32;//set default
	   m_clientEngine				= new CClientModule(m_hWnd,0,0,m_curModeWidth,m_curModeHieght,m_curModeDepth,FALSE,FALSE,TRUE,TRUE,TRUE,TRUE);*/
     #endif
     m_gStatus					= m_clientEngine->GetGlobalVarInterface();
     m_userControls				= m_clientEngine->GetUserControllerInterface();
	 m_sceneAssembler			= m_clientEngine->GetSceneAssemblerInterface();
	 m_cNetworkObject			= m_clientEngine->GetNetClientInterface();
	 m_menuManagement			= m_clientEngine->GetMenuManagerInterface();
	 m_rendermanagerEd			= m_clientEngine->GetRenderManagerInterface();
		 
	 //CreateScene();

	 return 0;
}

void RMWin::OnActivate(UINT state, CWnd* other, BOOL minimize)
{
//obsolete
}

void RMWin::OnMouseMove( UINT state, CPoint point )
{
	mousestate = state;
	mousex = point.x;
	mousey = point.y;
}

BOOL RMWin::OnEraseBkgnd( CDC* pDC ) 
{

	COLORREF bgcolor;
	bgcolor=RGB(0,0,0);

	CBrush br( bgcolor );
	CRect rc;
	GetClientRect(&rc);
	pDC->FillRect(&rc, &br);
	return FALSE;
}

void RMWin::OnDestroy()
{
     FreeAllThenClose();	
}


void RMWin::OnMove(int x, int y) 
{
	
	CFrameWnd::OnMove(x, y);
	
}

void RMWin::OnSize(UINT type, int cx, int cy)
{
	CFrameWnd::OnSize(type, cx, cy);
    m_szXMonitor  = cx;
    m_szYMonitor  = cy;
	m_windowStateGL = type;
	
//obsolete
}

LRESULT RMWin::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	

	return CFrameWnd::WindowProc(message, wParam, lParam);
}

void RMWin::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	if(m_clientEngine)
	   m_clientEngine->ProcessAnalogKeyboardMsgs(nChar);

#ifndef _ClientOutput
	// F9 or Tilde (`): toggle between Editor Mouse and 3D Walk/Look mode
	if(nChar == VK_F9 || nChar == VK_OEM_3)
	{
		if(m_clientEngine && m_clientEngine->m_initObject && m_clientEngine->m_initObject->m_gStatus)
		{
			DxDeviceClass* dx = m_clientEngine->m_initObject->m_gStatus->m_dxRenderer;
			if(dx)
			{
				if(dx->m_showMouse)
					dx->DisableMouse();
				else
					dx->EnableMouse();
			}
		}
	}
#endif

	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void RMWin::OnLButtonDown(UINT nFlags, CPoint point)
{
    if(m_clientEngine)
    if(m_showEditor == TRUE)
	{
		if(m_curModeWidth >= point.x)
			if(m_curModeHieght >= point.y)
		       m_clientEngine->m_initObject->m_gStatus->m_dxRenderer->DisableMouse();

	}

	CFrameWnd::OnLButtonDown(nFlags, point);
}
