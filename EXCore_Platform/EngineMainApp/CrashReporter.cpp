#include "stdafx.h"
#include "CrashReporter.h"
#include <dbghelp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <exception>
#include <atlconv.h>
#include <signal.h>

#pragma comment(lib, "dbghelp.lib")

static char g_logPath[MAX_PATH] = {0};
static LONG g_inHandler = 0;

static char g_appName[64] = "EngineX";   // exe name without extension (EngineX / EndlessAges / AxserverApp)

static void LogPath()
{
    if (g_logPath[0]) return;
    GetModuleFileNameA(NULL, g_logPath, MAX_PATH);
    {
        const char* base = strrchr(g_logPath, '\\'); base = base ? base + 1 : g_logPath;
        strncpy_s(g_appName, base, _TRUNCATE);
        char* dot = strrchr(g_appName, '.'); if (dot) *dot = 0;
    }
    char* p = strrchr(g_logPath, '\\');
    if (p) *(p + 1) = 0; else g_logPath[0] = 0;
    strcat_s(g_logPath, MAX_PATH, "CrashLog.txt");
}

void CrashLogLine(const char* text)
{
    LogPath();
    FILE* f = NULL;
    if (fopen_s(&f, g_logPath, "a") != 0 || !f) return;
    SYSTEMTIME st; GetLocalTime(&st);
    fprintf(f, "[%02d:%02d:%02d] %s\n", st.wHour, st.wMinute, st.wSecond, text);
    fclose(f);
}

static void ModuleOf(DWORD64 addr, char* out, size_t cb, DWORD64* offset)
{
    HMODULE hm = NULL;
    out[0] = 0; *offset = 0;
    if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                           (LPCSTR)(ULONG_PTR)addr, &hm) && hm)
    {
        char full[MAX_PATH] = {0};
        GetModuleFileNameA(hm, full, MAX_PATH);
        const char* base = strrchr(full, '\\');
        strcpy_s(out, cb, base ? base + 1 : full);
        *offset = addr - (DWORD64)(ULONG_PTR)hm;
    }
}

static void DescribeAddress(FILE* f, DWORD64 addr)
{
    char mod[MAX_PATH]; DWORD64 off = 0;
    ModuleOf(addr, mod, sizeof(mod), &off);
    fprintf(f, "  0x%08I64X  %s+0x%I64X", addr, mod[0] ? mod : "?", off);

    char symBuf[sizeof(SYMBOL_INFO) + 512] = {0};
    SYMBOL_INFO* sym = (SYMBOL_INFO*)symBuf;
    sym->SizeOfStruct = sizeof(SYMBOL_INFO);
    sym->MaxNameLen = 511;
    DWORD64 disp = 0;
    if (SymFromAddr(GetCurrentProcess(), addr, &disp, sym))
        fprintf(f, "  %s+0x%I64X", sym->Name, disp);
    IMAGEHLP_LINE64 line = {0}; line.SizeOfStruct = sizeof(line);
    DWORD ldisp = 0;
    if (SymGetLineFromAddr64(GetCurrentProcess(), addr, &ldisp, &line))
        fprintf(f, "  (%s:%lu)", line.FileName, line.LineNumber);
    fprintf(f, "\n");
}

static const char* ExceptionName(DWORD code)
{
    switch (code)
    {
    case EXCEPTION_ACCESS_VIOLATION:        return "ACCESS_VIOLATION";
    case EXCEPTION_STACK_OVERFLOW:          return "STACK_OVERFLOW";
    case EXCEPTION_INT_DIVIDE_BY_ZERO:      return "INT_DIVIDE_BY_ZERO";
    case EXCEPTION_ILLEGAL_INSTRUCTION:     return "ILLEGAL_INSTRUCTION";
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:   return "ARRAY_BOUNDS_EXCEEDED";
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:      return "FLT_DIVIDE_BY_ZERO";
    case EXCEPTION_FLT_INVALID_OPERATION:   return "FLT_INVALID_OPERATION";
    case 0xE06D7363:                        return "C++ exception (uncaught)";
    default:                                return "unknown";
    }
}

static void WriteCrash(EXCEPTION_POINTERS* ep, const char* title)
{
    LogPath();
    FILE* f = NULL;
    if (fopen_s(&f, g_logPath, "a") != 0 || !f) return;

    SYSTEMTIME st; GetLocalTime(&st);
    fprintf(f, "\n==== %s  %04d-%02d-%02d %02d:%02d:%02d ====\n", title,
            st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
    SymInitialize(GetCurrentProcess(), NULL, TRUE);

    CONTEXT ctx;
    if (ep && ep->ExceptionRecord)
    {
        EXCEPTION_RECORD* er = ep->ExceptionRecord;
        fprintf(f, "Exception 0x%08lX (%s) at", er->ExceptionCode, ExceptionName(er->ExceptionCode));
        DescribeAddress(f, (DWORD64)(ULONG_PTR)er->ExceptionAddress);
        if (er->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && er->NumberParameters >= 2)
            fprintf(f, "  %s address 0x%08IX\n",
                    er->ExceptionInformation[0] == 0 ? "reading" :
                    er->ExceptionInformation[0] == 1 ? "writing" : "executing",
                    (ULONG_PTR)er->ExceptionInformation[1]);
        ctx = *ep->ContextRecord;
    }
    else
    {
        RtlCaptureContext(&ctx);
    }

    fprintf(f, "Stack:\n");
    STACKFRAME64 frame = {0};
#ifdef _M_IX86
    DWORD machine = IMAGE_FILE_MACHINE_I386;
    frame.AddrPC.Offset    = ctx.Eip; frame.AddrPC.Mode    = AddrModeFlat;
    frame.AddrFrame.Offset = ctx.Ebp; frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrStack.Offset = ctx.Esp; frame.AddrStack.Mode = AddrModeFlat;
#else
    DWORD machine = IMAGE_FILE_MACHINE_AMD64;
    frame.AddrPC.Offset    = ctx.Rip; frame.AddrPC.Mode    = AddrModeFlat;
    frame.AddrFrame.Offset = ctx.Rbp; frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrStack.Offset = ctx.Rsp; frame.AddrStack.Mode = AddrModeFlat;
#endif
    for (int i = 0; i < 48; i++)
    {
        if (!StackWalk64(machine, GetCurrentProcess(), GetCurrentThread(), &frame, &ctx,
                         NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL))
            break;
        if (frame.AddrPC.Offset == 0) break;
        DescribeAddress(f, frame.AddrPC.Offset);
    }
    fprintf(f, "Working dir: ");
    char cwd[MAX_PATH] = {0}; GetCurrentDirectoryA(MAX_PATH, cwd);
    fprintf(f, "%s\n", cwd);
    fclose(f);
}

static void ShowBox(const char* what)
{
    char msg[1024];
    sprintf_s(msg, "%s hit a fatal error (%s).\n\nDetails were written to:\n%s\n\nSend that file to get it fixed.", g_appName,
              what, g_logPath);
    MessageBoxA(NULL, msg, "Crash report written", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
}

static LONG WINAPI CrashFilter(EXCEPTION_POINTERS* ep)
{
    if (InterlockedCompareExchange(&g_inHandler, 1, 0) != 0)
        return EXCEPTION_EXECUTE_HANDLER;
    WriteCrash(ep, "CRASH (unhandled exception)");
    ShowBox(ep && ep->ExceptionRecord ? ExceptionName(ep->ExceptionRecord->ExceptionCode) : "unknown");
    return EXCEPTION_EXECUTE_HANDLER;
}

static void OnTerminate()
{
    if (InterlockedCompareExchange(&g_inHandler, 1, 0) == 0)
    {
        WriteCrash(NULL, "TERMINATE (uncaught C++ exception)");
        ShowBox("uncaught C++ exception");
    }
    TerminateProcess(GetCurrentProcess(), 3);
}

static void OnPureCall()
{
    if (InterlockedCompareExchange(&g_inHandler, 1, 0) == 0)
    {
        WriteCrash(NULL, "PURE VIRTUAL CALL");
        ShowBox("pure virtual call");
    }
    TerminateProcess(GetCurrentProcess(), 3);
}

static void OnInvalidParameter(const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, uintptr_t)
{
    if (InterlockedCompareExchange(&g_inHandler, 1, 0) == 0)
    {
        WriteCrash(NULL, "CRT INVALID PARAMETER");
        ShowBox("invalid CRT parameter");
    }
    TerminateProcess(GetCurrentProcess(), 3);
}

// First-chance hook: sees faults BEFORE any catch(...) / MFC handler swallows them,
// and sees heap-corruption reports (0xC0000374), which Windows raises and then
// kills the process without ever calling the top-level filter. Logs and continues
// the normal search, so handled exceptions still behave as before.
static LONG CALLBACK FirstChanceHook(EXCEPTION_POINTERS* ep)
{
    if (!ep || !ep->ExceptionRecord) return EXCEPTION_CONTINUE_SEARCH;
    DWORD code = ep->ExceptionRecord->ExceptionCode;
    switch (code)
    {
    case 0xC0000005: /* access violation */
    case 0xC0000374: /* heap corruption   */
    case 0xC00000FD: /* stack overflow    */
    case 0xC000001D: /* illegal instr     */
    case 0xC0000094: /* int divide by 0   */
    case 0xC000008C: /* array bounds      */
    case 0xC0000409: /* stack buffer overrun / fail-fast */
        if (InterlockedCompareExchange(&g_inHandler, 1, 0) == 0)
        {
            WriteCrash(ep, "FIRST-CHANCE FAULT (process may die right after this)");
            InterlockedExchange(&g_inHandler, 0);
        }
        break;
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

static void OnAbortSignal(int)
{
    if (InterlockedCompareExchange(&g_inHandler, 1, 0) == 0)
    {
        WriteCrash(NULL, "ABORT (CRT abort() - heap check / assertion / runtime failure)");
        ShowBox("runtime abort");
    }
    TerminateProcess(GetCurrentProcess(), 3);
}

static void OnNormalExit()
{
    CrashLogLine("process exiting through the normal exit path (no crash recorded)");
}

void InstallCrashReporter()
{
    LogPath();
    SetUnhandledExceptionFilter(CrashFilter);
    AddVectoredExceptionHandler(1, FirstChanceHook);
    signal(SIGABRT, OnAbortSignal);
    atexit(OnNormalExit);
    std::set_terminate(OnTerminate);
    _set_purecall_handler(OnPureCall);
    _set_invalid_parameter_handler(OnInvalidParameter);
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
    { char line[128]; sprintf_s(line, "%s started (crash reporter armed)", g_appName); CrashLogLine(line); }
}

void ReportMfcException(CException* e, const tagMSG* pMsg)
{
    char cls[128] = "CException";
    char err[512] = "";
    if (e)
    {
        CRuntimeClass* rc = e->GetRuntimeClass();
        if (rc && rc->m_lpszClassName) strcpy_s(cls, rc->m_lpszClassName);
        TCHAR buf[512] = {0};
        e->GetErrorMessage(buf, 512);
        strcpy_s(err, CT2A(buf));
    }
    char line[1024];
    sprintf_s(line, "MFC EXCEPTION %s reached the message pump during msg 0x%04X: %s",
              cls, pMsg ? pMsg->message : 0, err);
    CrashLogLine(line);
    WriteCrash(NULL, line);
    char msg[1400];
    sprintf_s(msg, "%s\n\nDetails were written to:\n%s", line, g_logPath);
    MessageBoxA(NULL, msg, "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
}

void ReportDlgBarFailure(const char* iddName, unsigned int idd, const char* member)
{
    char line[512];
    sprintf_s(line, "DIALOG BAR CREATE FAILED: %s (resource id %u) for %s - dialog template missing from the exe's resources? Main window creation aborted.",
              iddName, idd, member);
    CrashLogLine(line);
    char msg[700];
    sprintf_s(msg, "%s\n\nDetails were written to:\n%s", line, g_logPath);
    MessageBoxA(NULL, msg, "Startup error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
}
