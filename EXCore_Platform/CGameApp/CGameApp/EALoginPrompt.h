// EALoginPrompt.h — in-client login window for Endless Ages (replaces the launcher).
// Self-contained Win32 (no MFC resources, no extra libraries). Returns the login the
// player typed; MainFrm then saves it to coinit.tmp with the existing WriteCoinitFile.
#pragma once
#include <afx.h>

struct EALoginInfo
{
    CString username;
    CString password;
    CString gameName;       // kept from coinit.tmp (e.g. "ENDLESS")
    CString contentServer;  // host:port the client fetches content from (host + ":25858")
    CString gameServer;     // game-server host/IP
    BOOL    fullscreen;
    BOOL    save;           // remember the password
};

// Shows the modal login window, filled in from `info`.
// Returns TRUE if the player pressed Play (fields copied back into `info`),
// FALSE if they closed or quit.
BOOL EAShowLoginPrompt(EALoginInfo& info);
