// EALoginPrompt.cpp — see EALoginPrompt.h.
// A plain Win32 modal window: username, password, server, "remember me", "full screen",
// a Play button and a "Create an account" link that opens the server's web sign-up page.
#include "stdafx.h"
#include "EALoginPrompt.h"
#include <windows.h>
#include <shellapi.h>

namespace {

enum { ID_USER = 201, ID_PASS, ID_SAVE, ID_FULL, ID_SERVER, ID_PLAY, ID_QUIT, ID_NEW, ID_MSG };

EALoginInfo* g_info = NULL;
BOOL g_ok = FALSE;
HFONT g_fUi = NULL, g_fTitle = NULL, g_fSmall = NULL;
HBRUSH g_brBg = NULL, g_brEdit = NULL;
HWND g_user = NULL, g_pass = NULL, g_save = NULL, g_full = NULL, g_server = NULL, g_msg = NULL, g_play = NULL;

const COLORREF C_BG = RGB(18, 22, 30), C_TEXT = RGB(230, 233, 238), C_MUTED = RGB(150, 160, 175),
               C_ACCENT = RGB(217, 164, 65), C_EDIT = RGB(10, 13, 18);

HWND Mk(HWND p, const char* cls, const char* text, DWORD style, int x, int y, int w, int h, int id, HFONT f)
{
    DWORD ex = (cls && lstrcmpA(cls, "EDIT") == 0) ? WS_EX_CLIENTEDGE : 0;
    HWND c = CreateWindowExA(ex, cls, text, WS_CHILD | WS_VISIBLE | style, x, y, w, h, p,
                             (HMENU)(INT_PTR)id, GetModuleHandle(NULL), NULL);
    SendMessageA(c, WM_SETFONT, (WPARAM)f, TRUE);
    return c;
}

// "host" or "host:port" -> "host:25858" (the server's content port)
CString ContentFrom(CString server)
{
    int c = server.Find(':');
    CString host = (c >= 0) ? server.Left(c) : server;
    host.TrimLeft(); host.TrimRight();
    if (host.IsEmpty()) host = "127.0.0.1";
    return host + ":25858";
}

void DoPlay(HWND w)
{
    char u[128] = "", p[128] = "", srv[160] = "";
    GetWindowTextA(g_user, u, sizeof u);
    GetWindowTextA(g_pass, p, sizeof p);
    GetWindowTextA(g_server, srv, sizeof srv);
    CString user(u), pass(p), server(srv);
    user.TrimLeft(); user.TrimRight(); server.TrimLeft(); server.TrimRight();

    if (user.IsEmpty() || pass.IsEmpty()) { SetWindowTextA(g_msg, "Type your username and password."); return; }
    if (user.GetLength() > 48 || pass.GetLength() > 48) { SetWindowTextA(g_msg, "Username and password must be 48 characters or fewer."); return; }

    g_info->username = user;
    g_info->password = pass;
    g_info->save = (SendMessageA(g_save, BM_GETCHECK, 0, 0) == BST_CHECKED);
    g_info->fullscreen = (SendMessageA(g_full, BM_GETCHECK, 0, 0) == BST_CHECKED);
    if (!server.IsEmpty())
    {
        int c = server.Find(':');
        g_info->gameServer = (c >= 0) ? server.Left(c) : server;   // game port is fixed; host only
        g_info->contentServer = ContentFrom(server);
    }
    g_ok = TRUE;
    DestroyWindow(w);
}

LRESULT CALLBACK WndProc(HWND w, UINT m, WPARAM wp, LPARAM lp)
{
    switch (m)
    {
    case WM_CREATE:
    {
        int x = 30, wd = 300;
        Mk(w, "STATIC", "ENDLESS AGES", SS_CENTER, 0, 22, 360, 34, 0, g_fTitle);
        Mk(w, "STATIC", "Log in to play", SS_CENTER, 0, 58, 360, 20, 0, g_fSmall);
        Mk(w, "STATIC", "Username", 0, x, 92, wd, 18, 0, g_fSmall);
        g_user = Mk(w, "EDIT", g_info->username, ES_AUTOHSCROLL | WS_TABSTOP, x, 111, wd, 26, ID_USER, g_fUi);
        Mk(w, "STATIC", "Password", 0, x, 145, wd, 18, 0, g_fSmall);
        g_pass = Mk(w, "EDIT", g_info->save ? g_info->password : CString(""), ES_AUTOHSCROLL | ES_PASSWORD | WS_TABSTOP, x, 164, wd, 26, ID_PASS, g_fUi);
        g_save = Mk(w, "BUTTON", "Remember me", BS_AUTOCHECKBOX | WS_TABSTOP, x, 198, 140, 22, ID_SAVE, g_fSmall);
        g_full = Mk(w, "BUTTON", "Full screen", BS_AUTOCHECKBOX | WS_TABSTOP, x + 160, 198, 140, 22, ID_FULL, g_fSmall);
        SendMessageA(g_save, BM_SETCHECK, g_info->save ? BST_CHECKED : BST_UNCHECKED, 0);
        SendMessageA(g_full, BM_SETCHECK, g_info->fullscreen ? BST_CHECKED : BST_UNCHECKED, 0);
        g_msg = Mk(w, "STATIC", "", SS_CENTER, x, 226, wd, 20, ID_MSG, g_fSmall);
        g_play = Mk(w, "BUTTON", "Play", BS_DEFPUSHBUTTON | WS_TABSTOP, x, 250, 196, 34, ID_PLAY, g_fUi);
        Mk(w, "BUTTON", "Quit", WS_TABSTOP, x + 204, 250, 96, 34, ID_QUIT, g_fUi);
        Mk(w, "STATIC", "Server", 0, x, 298, 60, 18, 0, g_fSmall);
        g_server = Mk(w, "EDIT", g_info->gameServer, ES_AUTOHSCROLL | WS_TABSTOP, x + 60, 295, wd - 60, 22, ID_SERVER, g_fSmall);
        Mk(w, "BUTTON", "Create an account", BS_FLAT | WS_TABSTOP, x, 326, wd, 26, ID_NEW, g_fSmall);
        SetFocus(g_info->username.IsEmpty() ? g_user : g_pass);
        return 0;
    }
    case WM_CTLCOLORSTATIC:
    {
        HDC dc = (HDC)wp; SetBkColor(dc, C_BG);
        BOOL accent = ((HWND)lp == g_msg) || ((GetWindowLongA((HWND)lp, GWL_STYLE) & SS_CENTER) && GetDlgCtrlID((HWND)lp) == 0);
        SetTextColor(dc, accent ? C_ACCENT : C_MUTED);
        return (LRESULT)g_brBg;
    }
    case WM_CTLCOLOREDIT: { SetBkColor((HDC)wp, C_EDIT); SetTextColor((HDC)wp, C_TEXT); return (LRESULT)g_brEdit; }
    case WM_CTLCOLORBTN:  { SetBkColor((HDC)wp, C_BG); return (LRESULT)g_brBg; }
    case WM_ERASEBKGND:   { RECT r; GetClientRect(w, &r); FillRect((HDC)wp, &r, g_brBg); return 1; }
    case WM_COMMAND:
        switch (LOWORD(wp))
        {
        case ID_PLAY: case IDOK: DoPlay(w); return 0;
        case ID_QUIT: case IDCANCEL: g_ok = FALSE; DestroyWindow(w); return 0;
        case ID_NEW:
        {
            char srv[160] = ""; GetWindowTextA(g_server, srv, sizeof srv);
            CString server(srv); if (server.IsEmpty()) server = g_info->gameServer;
            CString url = "http://" + ContentFrom(server) + "/register";
            ShellExecuteA(w, "open", url, NULL, NULL, SW_SHOWNORMAL);
            return 0;
        }
        }
        break;
    case WM_CLOSE: g_ok = FALSE; DestroyWindow(w); return 0;
    case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProcA(w, m, wp, lp);
}

} // namespace

BOOL EAShowLoginPrompt(EALoginInfo& info)
{
    g_info = &info; g_ok = FALSE;
    g_fUi = CreateFontA(-16, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, "Segoe UI");
    g_fSmall = CreateFontA(-13, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, "Segoe UI");
    g_fTitle = CreateFontA(-28, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, "Segoe UI");
    g_brBg = CreateSolidBrush(C_BG); g_brEdit = CreateSolidBrush(C_EDIT);

    WNDCLASSA wc; ZeroMemory(&wc, sizeof wc);
    wc.lpfnWndProc = WndProc; wc.hInstance = GetModuleHandle(NULL); wc.lpszClassName = "EALoginPrompt";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); wc.hbrBackground = g_brBg;
    wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(128));
    RegisterClassA(&wc);

    RECT r; r.left = 0; r.top = 0; r.right = 360; r.bottom = 372;
    AdjustWindowRect(&r, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);
    int ww = r.right - r.left, wh = r.bottom - r.top;
    HWND w = CreateWindowA("EALoginPrompt", "Endless Ages", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
                           (GetSystemMetrics(SM_CXSCREEN) - ww) / 2, (GetSystemMetrics(SM_CYSCREEN) - wh) / 2,
                           ww, wh, NULL, NULL, wc.hInstance, NULL);
    SetForegroundWindow(w);

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0) > 0)
        if (!IsDialogMessageA(w, &msg)) { TranslateMessage(&msg); DispatchMessageA(&msg); }

    UnregisterClassA("EALoginPrompt", wc.hInstance);
    DeleteObject(g_fUi); DeleteObject(g_fSmall); DeleteObject(g_fTitle);
    DeleteObject(g_brBg); DeleteObject(g_brEdit);
    g_fUi = g_fTitle = g_fSmall = NULL; g_brBg = g_brEdit = NULL;
    return g_ok;
}
