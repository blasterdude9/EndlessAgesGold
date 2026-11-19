#pragma once
// Crash reporter for EngineX.
// Catches hard crashes (access violations etc.), uncaught C++ exceptions,
// CRT invalid-parameter / pure-call faults and MFC CExceptions, writes
// CrashLog.txt next to EngineX.exe (symbolised when EngineX.pdb is present)
// and shows a message box so the app never just "vanishes".
class CException;
struct tagMSG;

void InstallCrashReporter();
// Log an MFC exception caught by the message-pump (see RMApp::ProcessWndProcException).
void ReportMfcException(CException* e, const tagMSG* pMsg);
// Append one line to CrashLog.txt (used for breadcrumbs from anywhere).
void CrashLogLine(const char* text);
// A CDialogBar/CDialog::Create(IDD_...) failed in EngineXWin::OnCreate. Log + box,
// so a missing dialog template never again exits the app with no window.
void ReportDlgBarFailure(const char* iddName, unsigned int idd, const char* member);
