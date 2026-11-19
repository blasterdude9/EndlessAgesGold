//---------------------------------------------------------------------------
// Copyright (C) 1998, Interscope Ltd. All rights reserved.
// Reproduction or distribution of this program, or any portion of it, 
// is permitted only if this header is kept as it is.
// For more information, contact:
//
// Interscope Ltd., 5 Culturii St., 5th floor, 4800 Baia Mare, Romania
//    Phone/Fax: +40-62-215023
//    E-mail: office@interscope.ro
//
//   $Author: Levente Farkas $
//     $Date: 5/12/98 11:50p $
//  $Modtime: 4/27/98 6:51a $
// $Revision: 11 $
//  $Archive: /Interscope/Thebe/InstallMaster/Trace.Cpp $
// $Workfile: Trace.Cpp $
//-----------------------------------------------------------------------


#include "StdAfx.H"


#if defined(_DEBUG) || defined(__DEBUG__)

#include <StdIO.H>
#include <StdArg.H>
#include <TChar.H>

#ifdef __MFC__
#include <AfxPriv.H>
#endif

#include "AssertX.H"
#include "Trace.H"


//--- Debugee --------------------------------------------------------------

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#ifdef __MFC__
#define new DEBUG_NEW
#endif // __MFC__
#endif // _DEBUG


//--- Miscellaneous ------------------------------------------------------

#define MAX_TRACE_BUFF_SIZE   1024


//--- Current indent level -----------------------------------------------

#ifdef __cplusplus
int CTraceIndentLevel::m_nIndent =-1;
#endif

#if defined(_DEBUG) || defined(__DEBUG__)
//------------------------------------------------------------------------
// Pre     : 
// Post    : 
// Globals : 
// I/O     : 
// Task    : Format a printf style message and send it 2 the active debugger
//------------------------------------------------------------------------
void InterscopeTrace(LPCTSTR lpszFormat, ...)
{
    int   nBuf;
	TCHAR szBuffer[MAX_TRACE_BUFF_SIZE];

	va_list args;
	va_start(args,lpszFormat);

	nBuf =_vstprintf(szBuffer, lpszFormat, args);
	ASSERTX(nBuf < sizeof(szBuffer));

    va_end(args);

#ifdef __cplusplus
    // Indenting stuff (only available in C++ code)
	// Search 4 newlines and insert prefix before each one
	static BOOL bStartNewLine =TRUE;
	LPTSTR lpszNextLine;
	for(LPTSTR lpszStart =szBuffer; *lpszStart; lpszStart =lpszNextLine+1)
    {
		if(bStartNewLine)
        {
            for(int i =0; i < CTraceIndentLevel::m_nIndent; i++)
                OutputDebugString(_T("   "));

			bStartNewLine =FALSE;
		}

        #ifndef _UNICODE
		lpszNextLine =strchr(lpszStart,'\n');
        #else
        lpszNextLine =wcschr(lpszStart,'\n');
        #endif

		if(lpszNextLine)
        {
            // Terminate string at newline
			*lpszNextLine ='\0';
			bStartNewLine =TRUE;
		}

        OutputDebugString(lpszStart);
		if(!lpszNextLine)
			break;
		OutputDebugString(_T("\n"));
	}
#else
    // Standard C trace code
	_tprintf(szBuffer);
    OutputDebugString(szBuffer);
#endif
}
#endif

#if defined(__MFC__) && (defined(_DEBUG) || defined(__DEBUG__))
//---------------------------------------------------------------------------
// Pre     : 
// Post    : 
// Globals : 
// I/O     : 
// Task    : Get window name in the form classname[HWND,title]
//           Searches all the parents for a window with a title
//---------------------------------------------------------------------------
CString DebugName(CWnd *pWnd)
{
	CString sTitle;
	HWND hwnd =pWnd->GetSafeHwnd();
	if(hwnd == NULL)
		sTitle =_T("NULL");
	else if(!::IsWindow(hwnd))
		sTitle =_T("[bad window]");
	else
    {
		sTitle =_T("[no title]");
		for(CWnd *pw =pWnd; pw; pw =pw->GetParent())
        {
			if(pw->GetWindowTextLength() > 0)
            {
				pw->GetWindowText(sTitle);
				break;
			}
		}
	}

	CString s;
	s.Format(_T("%s[0x%04x,\"%s\"]"), 
		     pWnd ? pWnd->GetRuntimeClass()->m_lpszClassName : _T("NULL"),
		     hwnd,
             (LPCTSTR)sTitle);

	return s;
}
#endif

#if defined(__MFC__) && (defined(_DEBUG) || defined(__DEBUG__))
// Human readable Windows message names
struct
{
	UINT	msg;
	LPCTSTR	name;
} MsgData[] =
{
	{ WM_CREATE,_T("WM_CREATE") },
	{ WM_DESTROY,_T("WM_DESTROY") },
	{ WM_MOVE,_T("WM_MOVE") },
	{ WM_SIZE,_T("WM_SIZE") },
	{ WM_ACTIVATE,_T("WM_ACTIVATE") },
	{ WM_SETFOCUS,_T("WM_SETFOCUS") },
	{ WM_KILLFOCUS,_T("WM_KILLFOCUS") },
	{ WM_ENABLE,_T("WM_ENABLE") },
	{ WM_SETREDRAW,_T("WM_SETREDRAW") },
	{ WM_SETTEXT,_T("WM_SETTEXT") },
	{ WM_GETTEXT,_T("WM_GETTEXT") },
	{ WM_GETTEXTLENGTH,_T("WM_GETTEXTLENGTH") },
	{ WM_PAINT,_T("WM_PAINT") },
	{ WM_CLOSE,_T("WM_CLOSE") },
	{ WM_QUERYENDSESSION,_T("WM_QUERYENDSESSION") },
	{ WM_QUIT,_T("WM_QUIT") },
	{ WM_QUERYOPEN,_T("WM_QUERYOPEN") },
	{ WM_ERASEBKGND,_T("WM_ERASEBKGND") },
	{ WM_SYSCOLORCHANGE,_T("WM_SYSCOLORCHANGE") },
	{ WM_ENDSESSION,_T("WM_ENDSESSION") },
	{ WM_SHOWWINDOW,_T("WM_SHOWWINDOW") },
	{ WM_WININICHANGE,_T("WM_WININICHANGE") },
	{ WM_SETTINGCHANGE,_T("WM_SETTINGCHANGE") },
	{ WM_DEVMODECHANGE,_T("WM_DEVMODECHANGE") },
	{ WM_ACTIVATEAPP,_T("WM_ACTIVATEAPP") },
	{ WM_FONTCHANGE,_T("WM_FONTCHANGE") },
	{ WM_TIMECHANGE,_T("WM_TIMECHANGE") },
	{ WM_CANCELMODE,_T("WM_CANCELMODE") },
	{ WM_SETCURSOR,_T("WM_SETCURSOR") },
	{ WM_MOUSEACTIVATE,_T("WM_MOUSEACTIVATE") },
	{ WM_CHILDACTIVATE,_T("WM_CHILDACTIVATE") },
	{ WM_QUEUESYNC,_T("WM_QUEUESYNC") },
	{ WM_GETMINMAXINFO,_T("WM_GETMINMAXINFO") },
	{ WM_PAINTICON,_T("WM_PAINTICON") },
	{ WM_ICONERASEBKGND,_T("WM_ICONERASEBKGND") },
	{ WM_NEXTDLGCTL,_T("WM_NEXTDLGCTL") },
	{ WM_SPOOLERSTATUS,_T("WM_SPOOLERSTATUS") },
	{ WM_DRAWITEM,_T("WM_DRAWITEM") },
	{ WM_MEASUREITEM,_T("WM_MEASUREITEM") },
	{ WM_DELETEITEM,_T("WM_DELETEITEM") },
	{ WM_VKEYTOITEM,_T("WM_VKEYTOITEM") },
	{ WM_CHARTOITEM,_T("WM_CHARTOITEM") },
	{ WM_SETFONT,_T("WM_SETFONT") },
	{ WM_GETFONT,_T("WM_GETFONT") },
	{ WM_SETHOTKEY,_T("WM_SETHOTKEY") },
	{ WM_GETHOTKEY,_T("WM_GETHOTKEY") },
	{ WM_QUERYDRAGICON,_T("WM_QUERYDRAGICON") },
	{ WM_COMPAREITEM,_T("WM_COMPAREITEM") },
	{ WM_COMPACTING,_T("WM_COMPACTING") },
	{ WM_COMMNOTIFY,_T("WM_COMMNOTIFY") },
	{ WM_WINDOWPOSCHANGING,_T("WM_WINDOWPOSCHANGING") },
	{ WM_WINDOWPOSCHANGED,_T("WM_WINDOWPOSCHANGED") },
	{ WM_POWER,_T("WM_POWER") },
	{ WM_COPYDATA,_T("WM_COPYDATA") },
	{ WM_CANCELJOURNAL,_T("WM_CANCELJOURNAL") },
#if(WINVER >= 0x0400)
	{ WM_NOTIFY,_T("WM_NOTIFY") },
	{ WM_INPUTLANGCHANGEREQUEST,_T("WM_INPUTLANGCHANGEREQUEST") },
	{ WM_INPUTLANGCHANGE,_T("WM_INPUTLANGCHANGE") },
	{ WM_TCARD,_T("WM_TCARD") },
	{ WM_HELP,_T("WM_HELP") },
	{ WM_USERCHANGED,_T("WM_USERCHANGED") },
	{ WM_NOTIFYFORMAT,_T("WM_NOTIFYFORMAT") },
	{ WM_CONTEXTMENU,_T("WM_CONTEXTMENU") },
	{ WM_STYLECHANGING,_T("WM_STYLECHANGING") },
	{ WM_STYLECHANGED,_T("WM_STYLECHANGED") },
	{ WM_DISPLAYCHANGE,_T("WM_DISPLAYCHANGE") },
	{ WM_GETICON,_T("WM_GETICON") },
	{ WM_SETICON,_T("WM_SETICON") },
#endif // WINVER >= 0x0400
	{ WM_NCCREATE,_T("WM_NCCREATE") },
	{ WM_NCDESTROY,_T("WM_NCDESTROY") },
	{ WM_NCCALCSIZE,_T("WM_NCCALCSIZE") },
	{ WM_NCHITTEST,_T("WM_NCHITTEST") },
	{ WM_NCPAINT,_T("WM_NCPAINT") },
	{ WM_NCACTIVATE,_T("WM_NCACTIVATE") },
	{ WM_GETDLGCODE,_T("WM_GETDLGCODE") },
	{ WM_NCMOUSEMOVE,_T("WM_NCMOUSEMOVE") },
	{ WM_NCLBUTTONDOWN,_T("WM_NCLBUTTONDOWN") },
	{ WM_NCLBUTTONUP,_T("WM_NCLBUTTONUP") },
	{ WM_NCLBUTTONDBLCLK,_T("WM_NCLBUTTONDBLCLK") },
	{ WM_NCRBUTTONDOWN,_T("WM_NCRBUTTONDOWN") },
	{ WM_NCRBUTTONUP,_T("WM_NCRBUTTONUP") },
	{ WM_NCRBUTTONDBLCLK,_T("WM_NCRBUTTONDBLCLK") },
	{ WM_NCMBUTTONDOWN,_T("WM_NCMBUTTONDOWN") },
	{ WM_NCMBUTTONUP,_T("WM_NCMBUTTONUP") },
	{ WM_NCMBUTTONDBLCLK,_T("WM_NCMBUTTONDBLCLK") },
	{ WM_KEYDOWN,_T("WM_KEYDOWN") },
	{ WM_KEYUP,_T("WM_KEYUP") },
	{ WM_CHAR,_T("WM_CHAR") },
	{ WM_DEADCHAR,_T("WM_DEADCHAR") },
	{ WM_SYSKEYDOWN,_T("WM_SYSKEYDOWN") },
	{ WM_SYSKEYUP,_T("WM_SYSKEYUP") },
	{ WM_SYSCHAR,_T("WM_SYSCHAR") },
	{ WM_SYSDEADCHAR,_T("WM_SYSDEADCHAR") },
	{ WM_KEYDOWN,_T("WM_KEYDOWN") },
	{ WM_KEYUP,_T("WM_KEYUP") },
	{ WM_CHAR,_T("WM_CHAR") },
	{ WM_DEADCHAR,_T("WM_DEADCHAR") },
	{ WM_SYSKEYDOWN,_T("WM_SYSKEYDOWN") },
	{ WM_SYSKEYUP,_T("WM_SYSKEYUP") },
	{ WM_SYSCHAR,_T("WM_SYSCHAR") },
	{ WM_SYSDEADCHAR,_T("WM_SYSDEADCHAR") },
#if(WINVER >= 0x0400)
	{ WM_IME_STARTCOMPOSITION,_T("WM_IME_STARTCOMPOSITION") },
	{ WM_IME_ENDCOMPOSITION,_T("WM_IME_ENDCOMPOSITION") },
	{ WM_IME_COMPOSITION,_T("WM_IME_COMPOSITION") },
	{ WM_IME_KEYLAST,_T("WM_IME_KEYLAST") },
#endif // WINVER >= 0x0400
	{ WM_INITDIALOG,_T("WM_INITDIALOG") },
	{ WM_COMMAND,_T("WM_COMMAND") },
	{ WM_SYSCOMMAND,_T("WM_SYSCOMMAND") },
	{ WM_TIMER,_T("WM_TIMER") },
	{ WM_HSCROLL,_T("WM_HSCROLL") },
	{ WM_VSCROLL,_T("WM_VSCROLL") },
	{ WM_INITMENU,_T("WM_INITMENU") },
	{ WM_INITMENUPOPUP,_T("WM_INITMENUPOPUP") },
	{ WM_MENUSELECT,_T("WM_MENUSELECT") },
	{ WM_MENUCHAR,_T("WM_MENUCHAR") },
	{ WM_ENTERIDLE,_T("WM_ENTERIDLE") },
	{ WM_CTLCOLORMSGBOX,_T("WM_CTLCOLORMSGBOX") },
	{ WM_CTLCOLOREDIT,_T("WM_CTLCOLOREDIT") },
	{ WM_CTLCOLORLISTBOX,_T("WM_CTLCOLORLISTBOX") },
	{ WM_CTLCOLORBTN,_T("WM_CTLCOLORBTN") },
	{ WM_CTLCOLORDLG,_T("WM_CTLCOLORDLG") },
	{ WM_CTLCOLORSCROLLBAR,_T("WM_CTLCOLORSCROLLBAR") },
	{ WM_CTLCOLORSTATIC,_T("WM_CTLCOLORSTATIC") },
	{ WM_MOUSEMOVE,_T("WM_MOUSEMOVE") },
	{ WM_LBUTTONDOWN,_T("WM_LBUTTONDOWN") },
	{ WM_LBUTTONUP,_T("WM_LBUTTONUP") },
	{ WM_LBUTTONDBLCLK,_T("WM_LBUTTONDBLCLK") },
	{ WM_RBUTTONDOWN,_T("WM_RBUTTONDOWN") },
	{ WM_RBUTTONUP,_T("WM_RBUTTONUP") },
	{ WM_RBUTTONDBLCLK,_T("WM_RBUTTONDBLCLK") },
	{ WM_MBUTTONDOWN,_T("WM_MBUTTONDOWN") },
	{ WM_MBUTTONUP,_T("WM_MBUTTONUP") },
	{ WM_MBUTTONDBLCLK,_T("WM_MBUTTONDBLCLK") },
	{ WM_MOUSEMOVE,_T("WM_MOUSEMOVE") },
	{ WM_LBUTTONDOWN,_T("WM_LBUTTONDOWN") },
	{ WM_LBUTTONUP,_T("WM_LBUTTONUP") },
	{ WM_LBUTTONDBLCLK,_T("WM_LBUTTONDBLCLK") },
	{ WM_RBUTTONDOWN,_T("WM_RBUTTONDOWN") },
	{ WM_RBUTTONUP,_T("WM_RBUTTONUP") },
	{ WM_RBUTTONDBLCLK,_T("WM_RBUTTONDBLCLK") },
	{ WM_MBUTTONDOWN,_T("WM_MBUTTONDOWN") },
	{ WM_MBUTTONUP,_T("WM_MBUTTONUP") },
	{ WM_MBUTTONDBLCLK,_T("WM_MBUTTONDBLCLK") },
	{ WM_PARENTNOTIFY,_T("WM_PARENTNOTIFY") },
	{ WM_ENTERMENULOOP,_T("WM_ENTERMENULOOP") },
	{ WM_EXITMENULOOP,_T("WM_EXITMENULOOP") },
#if(WINVER >= 0x0400)
	{ WM_NEXTMENU,_T("WM_NEXTMENU") },
	{ WM_SIZING,_T("WM_SIZING") },
	{ WM_CAPTURECHANGED,_T("WM_CAPTURECHANGED") },
	{ WM_MOVING,_T("WM_MOVING") },
	{ WM_POWERBROADCAST,_T("WM_POWERBROADCAST") },
	{ WM_DEVICECHANGE,_T("WM_DEVICECHANGE") },
	{ WM_IME_SETCONTEXT,_T("WM_IME_SETCONTEXT") },
	{ WM_IME_NOTIFY,_T("WM_IME_NOTIFY") },
	{ WM_IME_CONTROL,_T("WM_IME_CONTROL") },
	{ WM_IME_COMPOSITIONFULL,_T("WM_IME_COMPOSITIONFULL") },
	{ WM_IME_SELECT,_T("WM_IME_SELECT") },
	{ WM_IME_CHAR,_T("WM_IME_CHAR") },
	{ WM_IME_KEYDOWN,_T("WM_IME_KEYDOWN") },
	{ WM_IME_KEYUP,_T("WM_IME_KEYUP") },
#endif // WINVER >= 0x0400
	{ WM_MDICREATE,_T("WM_MDICREATE") },
	{ WM_MDIDESTROY,_T("WM_MDIDESTROY") },
	{ WM_MDIACTIVATE,_T("WM_MDIACTIVATE") },
	{ WM_MDIRESTORE,_T("WM_MDIRESTORE") },
	{ WM_MDINEXT,_T("WM_MDINEXT") },
	{ WM_MDIMAXIMIZE,_T("WM_MDIMAXIMIZE") },
	{ WM_MDITILE,_T("WM_MDITILE") },
	{ WM_MDICASCADE,_T("WM_MDICASCADE") },
	{ WM_MDIICONARRANGE,_T("WM_MDIICONARRANGE") },
	{ WM_MDIGETACTIVE,_T("WM_MDIGETACTIVE") },
	{ WM_MDISETMENU,_T("WM_MDISETMENU") },
	{ WM_ENTERSIZEMOVE,_T("WM_ENTERSIZEMOVE") },
	{ WM_EXITSIZEMOVE,_T("WM_EXITSIZEMOVE") },
	{ WM_DROPFILES,_T("WM_DROPFILES") },
	{ WM_MDIREFRESHMENU,_T("WM_MDIREFRESHMENU") },
	{ WM_CUT,_T("WM_CUT") },
	{ WM_COPY,_T("WM_COPY") },
	{ WM_PASTE,_T("WM_PASTE") },
	{ WM_CLEAR,_T("WM_CLEAR") },
	{ WM_UNDO,_T("WM_UNDO") },
	{ WM_RENDERFORMAT,_T("WM_RENDERFORMAT") },
	{ WM_RENDERALLFORMATS,_T("WM_RENDERALLFORMATS") },
	{ WM_DESTROYCLIPBOARD,_T("WM_DESTROYCLIPBOARD") },
	{ WM_DRAWCLIPBOARD,_T("WM_DRAWCLIPBOARD") },
	{ WM_PAINTCLIPBOARD,_T("WM_PAINTCLIPBOARD") },
	{ WM_VSCROLLCLIPBOARD,_T("WM_VSCROLLCLIPBOARD") },
	{ WM_SIZECLIPBOARD,_T("WM_SIZECLIPBOARD") },
	{ WM_ASKCBFORMATNAME,_T("WM_ASKCBFORMATNAME") },
	{ WM_CHANGECBCHAIN,_T("WM_CHANGECBCHAIN") },
	{ WM_HSCROLLCLIPBOARD,_T("WM_HSCROLLCLIPBOARD") },
	{ WM_QUERYNEWPALETTE,_T("WM_QUERYNEWPALETTE") },
	{ WM_PALETTEISCHANGING,_T("WM_PALETTEISCHANGING") },
	{ WM_PALETTECHANGED,_T("WM_PALETTECHANGED") },
	{ WM_HOTKEY,_T("WM_HOTKEY") },
#if(WINVER >= 0x0400)
	{ WM_PRINT,_T("WM_PRINT") },
	{ WM_PRINTCLIENT,_T("WM_PRINTCLIENT") },
#endif // WINVER >= 0x0400
    // Below are MFC messages
	{ WM_QUERYAFXWNDPROC,_T("*WM_QUERYAFXWNDPROC") },
	{ WM_SIZEPARENT,_T("*WM_SIZEPARENT") },
	{ WM_SETMESSAGESTRING,_T("*WM_SETMESSAGESTRING") },
	{ WM_IDLEUPDATECMDUI,_T("*WM_IDLEUPDATECMDUI") },
	{ WM_INITIALUPDATE,_T("*WM_INITIALUPDATE") },
	{ WM_COMMANDHELP,_T("*WM_COMMANDHELP") },
	{ WM_HELPHITTEST,_T("*WM_HELPHITTEST") },
	{ WM_EXITHELPMODE,_T("*WM_EXITHELPMODE") },
	{ WM_RECALCPARENT,_T("*WM_RECALCPARENT") },
	{ WM_SIZECHILD,_T("*WM_SIZECHILD") },
	{ WM_KICKIDLE,_T("*WM_KICKIDLE") },
	{ WM_QUERYCENTERWND,_T("*WM_QUERYCENTERWND") },
	{ WM_DISABLEMODAL,_T("*WM_DISABLEMODAL") },
	{ WM_FLOATSTATUS,_T("*WM_FLOATSTATUS") },
	{ WM_ACTIVATETOPLEVEL,_T("*WM_ACTIVATETOPLEVEL") },
	{ WM_QUERY3DCONTROLS,_T("*WM_QUERY3DCONTROLS") },
	{ WM_SOCKET_NOTIFY,_T("*WM_SOCKET_NOTIFY") },
	{ WM_SOCKET_DEAD,_T("*WM_SOCKET_DEAD") },
	{ WM_POPMESSAGESTRING,_T("*WM_POPMESSAGESTRING") },
	{ WM_OCC_LOADFROMSTREAM,_T("*WM_OCC_LOADFROMSTREAM") },
	{ WM_OCC_LOADFROMSTORAGE,_T("*WM_OCC_LOADFROMSTORAGE") },
	{ WM_OCC_INITNEW,_T("*WM_OCC_INITNEW") },
	{ WM_QUEUE_SENTINEL,_T("*WM_QUEUE_SENTINEL") },
	{ 0,NULL }
};
#endif

// This class is basically just an array of 1024 strings,
// the names of each WM_ message
// The constructor initializes it

#if defined(__MFC__) && (defined(_DEBUG) || defined(__DEBUG__))
class CWndMsgMap
{
// Data members
private:
	static LPCTSTR Names[];

// Implementation
public:
	CWndMsgMap();
	CString GetMsgName(UINT msg);
};
LPCTSTR CWndMsgMap::Names[WM_USER];	// Name of each WM_ message
#endif

#if defined(__MFC__) && (defined(_DEBUG) || defined(__DEBUG__))
//---------------------------------------------------------------------------
// Pre     : 
// Post    : 
// Globals : 
// I/O     : 
// Task    : Initialize array from sparse data
//---------------------------------------------------------------------------
CWndMsgMap::CWndMsgMap()
{
	// Copy sparse MsgData into table
	ZeroMemory(Names,sizeof(Names));
	for(int i=0; MsgData[i].msg; i++)		
		Names[MsgData[i].msg] =MsgData[i].name;
}
#endif

#if defined(__MFC__) && (defined(_DEBUG) || defined(__DEBUG__))
//---------------------------------------------------------------------------
// Pre     : 
// Post    : 
// Globals : 
// I/O     : 
// Task    : Get the name of a WM_ message
//---------------------------------------------------------------------------
CString CWndMsgMap::GetMsgName(UINT msg)
{
	CString name;
	if(msg >= WM_USER)
		name.Format(_T("WM_USER+%d"), msg-WM_USER);
	else if(Names[msg])
		name =Names[msg];
	else
		name.Format(_T("0x%04x"),msg);

	return name;
}
#endif

#if defined(__MFC__) && (defined(_DEBUG) || defined(__DEBUG__))
//---------------------------------------------------------------------------
// Pre     : 
// Post    : 
// Globals : 
// I/O     : 
// Task    : Get the name of a WM_ message
//---------------------------------------------------------------------------
CString DebugName(UINT uMsg)
{
	static CWndMsgMap wndMsgMap; // Instantiate 1st time called
	return wndMsgMap.GetMsgName(uMsg);
}
#endif


#endif
